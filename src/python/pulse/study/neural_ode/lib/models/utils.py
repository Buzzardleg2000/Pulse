###########################
# Modified from: https://github.com/YuliaRubanova/latent_ode by Kirin Ciao
###########################
import os
import sys
import csv
import time
import logging
import pickle
import math
import torch
import torch.nn as nn
import torchcde
import numpy as np
import pandas as pd
import ubelt as ub
import config as cf
from dataclasses import dataclass
from typing import Optional, List, Literal


def get_device(tensor):
    device = torch.device('cpu')
    if tensor.is_cuda:
        device = tensor.get_device()
    return device


def set_seed(seed: int = 47):
    np.random.seed(seed)
    # random.seed(seed)
    torch.manual_seed(seed)
    # GPU
    # torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.random.manual_seed(seed)


@dataclass
class ProcessDataset():

    # common
    load_data: bool = True
    ode_time: Literal['Concat', 'Mean', 'No'] = 'No'
    produce_intercoeffs: bool = False
    x_data_with_time: bool = False
    x_y_points = None
    x_points = None
    y_delay = 0
    y_dim_list = None
    dataset: str

    # split_train_val_test
    csv_list: Optional[List[str]] = None
    continue_params: Optional[List[str]] = None
    parse_dates: Optional[List[str]] = None
    glob_params: Optional[List[str]] = None

    # cut_and_split_data
    stride: int = 1
    train_fraq: float = 0.8
    val_fraq: Optional[float] = None
    shuffle: bool = True
    n_start: int = 0

    # dataloader
    batch_size: int
    missing_rate: Optional[float]

    # ENTRYPOINT
    def get_data(self):
        data_pkl_file = ub.Path('data') / dataset / 'All_Data.pkl'
        #TRANS: Not the interpolation and Concat, only can be read directly
        if self.load_data and os.path.isfile(
                data_pkl_file
        ) and self.ode_time != 'Concat' and not self.produce_intercoeffs:
            return load_pickle(data_pkl_file)
        else:
            data_dict = self.split_train_val_test(dataset)
            train_dict, x_dims, y_dims = self.split_x_y(data_dict, 'train')
            val_dict, _, _ = self.split_x_y(data_dict, 'val')
            test_dict, _, _ = self.plit_x_y(data_dict, 'test')

            #TRANS: Not the interpolation and Concat, can be stored here
            if self.ode_time != 'Concat' and not self.produce_intercoeffs:
                data = (train_dict, val_dict, test_dict, x_dims, y_dims)
                save_items_to_pickle(data, data_pkl_file)
            #TRANS: TODO: Concat set batch_size after reading to the corresponding value = 1
        return train_dict, val_dict, test_dict, x_dims, y_dims

    def _split_train_val_test(self, dataset):
        #TRANS: To extract data from the original file and is divided into training set and test set (verification), pay attention to the new tensor. The device
        if 'csv_list' is None:  #TRANS: From the train. The pt and test. Pt in batch data extracted directly, not reorder
            train = torch.load('data/' + dataset + '/train.pt')
            test = torch.load('data/' + dataset + '/test.pt')
            train_time = train[:, :, :1]
            test_time = test[:, :, :1]
            train_data = train[:, :, 1:]
            test_data = test[:, :, 1:]
            data_dict = {
                'train_data': train_data,
                'test_data': test_data,
                'train_time': train_time,
                'test_time': test_time
            }
            if os.path.isfile('data/' + dataset + '/val.pt'):  #TRANS: Validation set, used to adjust the lr and other parameters
                val = torch.load('data/' + dataset + '/val.pt')
                val_time = val[:, :, :1]
                val_data = val[:, :, 1:]
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = None
                data_dict['val_time'] = None
            return data_dict
        else:  #TRANS: Read data from CSV, segmentation and classification training set/test set (training set and testing set in the same file)
            train_data = []
            val_data = []
            test_data = []
            train_time = []
            val_time = []
            test_time = []

            data_min = []
            data_max = []

            #TRANS: Because the Numpy support for string is not very good, so use nested list directly, must use 2 slices
            train_glob_list = []
            val_glob_list = []
            test_glob_list = []
            for csv_file in csv_list:
                csv_path = 'data/' + dataset + '/' + csv_file + '.csv'
                #TRANS: Replace the missing value: https://blog.csdn.net/qq_18351157/article/details/104993254, here to get rid of the missing value corresponding to the point
                origin_pd_data = pd.read_csv(csv_path,
                                             header=0).dropna(how='any')

                if continue_params is not None:
                    pd_continue_data = origin_pd_data[continue_params]
                else:
                    pd_continue_data = origin_pd_data  #TRANS: Read all labels, all, applicable to CSV file all variables are needed

                if parse_dates is not None:  #TRANS: Converts time variable to a floating point number
                    pd_timestamp = pd.to_datetime(origin_pd_data[parse_dates],
                                                  infer_datetime_format=True)
                    pd_timestamp = ((pd_timestamp - pd_timestamp.iloc[0]) /
                                    pd.Timedelta(1, 'd')).fillna(0).astype(
                                        float)  #TRANS: Each file time starting from 0, 1 day as unit 1
                    pd_timestamp = pd_timestamp.rename('time')
                    pd_continue_data = pd.concat(
                        [pd_timestamp, pd_continue_data],
                        axis=1)  #TRANS: Continue_data to guarantee on time as the first column

                #TRANS: TODO: data processing tag, add it to the scalar?
                # https://blog.csdn.net/qq_32863549/article/details/106972347
                # https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html?highlight=labelencoder#sklearn.preprocessing.LabelEncoder
                # TODO label_dict?
                if 'discrete_params' is not None:  #TRANS: To deal with tags inside the CSV data, such as wind, finally to merge into the tensor_data, in addition to save a label map?
                    pd_discrete_data = origin_pd_data[discrete_params]

                tensor_data = torch.tensor(data=pd_continue_data.values)

                #TRANS: Validation sets empty cases, the returned data and time are None
                (sw_train_data, sw_val_data, sw_test_data, sw_train_time,
                 sw_val_time, sw_test_time,
                 csv_data_min, csv_data_max) = cut_and_split_data(
                     tensor_data, cf.get())  #TRANS: In the training set and validation set according to shuffle
                train_data.append(sw_train_data)
                test_data.append(sw_test_data)
                train_time.append(sw_train_time)
                test_time.append(sw_test_time)
                val_data.append(sw_val_data)
                val_time.append(sw_val_time)
                data_min.append(csv_data_min)
                data_max.append(csv_data_max)

                if 'glob_params' is not None:  #TRANS: Used to handle the whole time series (including training and test sets) are the same data, such as sampling sites
                    glob_item = origin_pd_data[glob_params].iloc[
                        0].values.tolist()  #TRANS: This batch of the values are the same
                    train_glob_list.extend([glob_item] *
                                           sw_train_time.shape[0])
                    if sw_val_time is not None:
                        val_glob_list.extend([glob_item] *
                                             sw_val_time.shape[0])
                    else:
                        val_glob_list = None
                    test_glob_list.extend([glob_item] * sw_test_time.shape[0])

            train_data = torch.cat(train_data, dim=0)
            test_data = torch.cat(test_data, dim=0)
            train_time = torch.cat(train_time, dim=0)
            test_time = torch.cat(test_time, dim=0)
            data_dict = {
                'train_data': train_data,
                'test_data': test_data,
                'train_time': train_time,
                'test_time': test_time
            }

            if 'discrete_params' is not None:  #TRANS: Additional label data
                raise NotImplementedError

            if val_data[0] is not None:  #TRANS: Additional validation set
                val_data = torch.cat(val_data, dim=0)
                val_time = torch.cat(val_time, dim=0)
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time

            if 'glob_params' is not None:  #TRANS: Additional global data
                data_dict['train_glob_list'] = train_glob_list
                data_dict['val_glob_list'] = val_glob_list
                data_dict['test_glob_list'] = test_glob_list

            pickle_data = (
                x_data_with_time, pd_continue_data.columns.tolist()
                if x_data_with_time else pd_continue_data.columns.tolist()[1:],
                data_min, data_max)  #TRANS: TODO: save the mapping information
            save_items_to_pickle(pickle_data,
                                 'data/' + dataset + '/DataInfo.pkl')

            return data_dict

    def _cut_and_split_data(self,
                            data,
                            x_y_points=None,
                            stride=1,
                            train_fraq=0.8,
                            val_fraq=None,
                            shuffle=True,
                            n_start=0,
                            x_data_with_time=False):
        #TRANS: Dividing a sliding window
        if x_y_points is None:
            x_y_points = data.shape[0] // 100
        device = get_device(data)

        if x_data_with_time:
            data, data_min, data_max = normalize_data_minmax(data)
        else:
            data[:, 1:], data_min, data_max = normalize_data_minmax(
                data[:, 1:])  #TRANS: The normalize set to min - Max
        assert not torch.isnan(data).any()
        assert not torch.isnan(data_min).any()
        assert not torch.isnan(data_max).any()

        data_new = data[n_start:].clone().detach().unsqueeze(0)
        assert x_y_points < data_new.shape[1] - 1
        for i in range(1, x_y_points):
            zero_tensor = torch.zeros((i, data.shape[1]), device=device)
            tmp_tensor = torch.vstack(
                (data[i + n_start:], zero_tensor)).unsqueeze(0)
            data_new = torch.cat((data_new, tmp_tensor))
        data_new = data_new.permute(1, 0, 2)
        data_new = data_new[:-(x_y_points - 1)]  #TRANS: Cut off the end of a few slices with zero component
        if stride > 1 and data_new.shape[0] > stride:
            data_select = data_new[0].unsqueeze(0)
            for j in range(stride, data_new.shape[0], stride):
                data_select = torch.cat(
                    (data_select, data_new[j].unsqueeze(0)))
            data_new = data_select
        if shuffle:  #TRANS: Reordering of sliding window
            idx = torch.randperm(data_new.shape[0])
            data_new = data_new[idx]

        time = data_new[:, :, :1]
        data = data_new[:, :, 1:]

        train_data, val_data, test_data, train_time, val_time, test_time = split_train_test_data_and_time(
            data, time, train_fraq, val_fraq)
        return train_data, val_data, test_data, train_time, val_time, test_time, data_min, data_max

    def _split_train_test_data_and_time(self,
                                        data,
                                        time,
                                        train_fraq=0.8,
                                        val_fraq=None):
        #TRANS: Divided into training set and test set, 1 d to divided the time period
        #TRANS: Regression test suite should overall is unknown, the first step from the training last time as input, and then gradually replace data_test and test_time segmentation
        data_size = data.shape[0]
        assert len(time.shape) == 3

        if val_fraq is None:
            train_point = int(data_size * train_fraq)
            train_data = data[:train_point]
            train_time = time[:train_point]
            test_data = data[train_point:]
            test_time = time[train_point:]
            val_data = None
            val_time = None
        else:
            train_point = int(data_size * train_fraq)
            val_point = int(data_size * (val_fraq + train_fraq))
            train_data = data[:train_point]
            train_time = time[:train_point]
            val_data = data[train_point:val_point]
            val_time = time[train_point:val_point]
            test_data = data[val_point:]
            test_time = time[val_point:]
        return train_data, val_data, test_data, train_time, val_time, test_time

    def _split_x_y(self,
                   data_dict,
                   data_type: Literal['train', 'val', 'test'],
                   x_data_with_time: bool,
                   x_y_points,
                   x_points=None,
                   y_delay=0,
                   y_dim_list=None,
                   arch):  # TODO
        if data_type + '_time' not in data_dict or data_dict[data_type +
                                                             '_time'] is None:
            return None
        if x_points is not None:
            x_points = min(x_points, x_y_points - 1)
        #TRANS: The autoregressive prediction, Autoencoder reverse solution to h0 first, then forward solution to the last point
        if y_dim_list is not None:
            y_dims = len(y_dim_list)
            all_dim_list = [
                i for i in range(data_dict[data_type + '_data'].shape[-1])
            ]
            x_dim_list = list(set(all_dim_list).difference(set(y_dim_list)))
            x_dims = len(x_dim_list)
            split_dict = {
                'x_data': data_dict[data_type + '_data'][:, :,
                                                         x_dim_list].clone(),
                'x_time': data_dict[data_type + '_time'].clone(),
                'y_data': data_dict[data_type + '_data'][:, :,
                                                         y_dim_list].clone(),
                'y_time': data_dict[data_type + '_time'].clone()
            }
            if arch in {'Seq2Seq', 'VAE'}:  #TRANS: Two kinds of architecture, using AutoEncoder need a small amount of x reverse solving until t0
                #TRANS: The linear small
                x_data_until = split_dict['x_data'][:, :1, :] - 1 / 10 * (
                    split_dict['x_data'][:, 1:2, :] -
                    split_dict['x_data'][:, :1, :])
                x_time_until = split_dict['x_time'][:, :1, :] - 1 / 10 * (
                    split_dict['x_time'][:, 1:2, :] -
                    split_dict['x_time'][:, :1, :])
                split_dict['x_data'] = torch.cat(
                    (split_dict['x_data'].flip(dims=[1]), x_data_until), dim=1)
                split_dict['x_time'] = torch.cat(
                    (split_dict['x_time'].flip(dims=[1]), x_time_until), dim=1)
        else:
            if x_points is None:
                #TRANS: Regression: 1/2 relationship of default in accordance with the observation point (input) and the observation point (output)
                x_points = data_dict[data_type + '_data'].shape[1] // 2
            #TRANS: Y_delay is used to predict in advance, the y move back from the total length of time
            split_dict = {
                'x_data':
                data_dict[data_type + '_data'][:, :x_points, :].clone(),
                'x_time':
                data_dict[data_type + '_time'][:, :x_points, :].clone(),
                'y_data':
                data_dict[data_type + '_data'][:,
                                               x_points + y_delay:, :].clone(),
                'y_time':
                data_dict[data_type + '_time'][:,
                                               x_points + y_delay:, :].clone()
            }
            x_dims = split_dict['x_data'].shape[-1]
            y_dims = split_dict['y_data'].shape[-1]

        if x_data_with_time:  #TRANS: Add the time sequence x
            split_dict['x_data'] = torch.cat(
                (split_dict['x_time'], split_dict['x_data']), dim=2)
            x_dims += 1

        return split_dict, x_dims, y_dims

    #TRANS: In the training process the data
    # ENTRYPOINT
    def masked_dataloader(self,
                          data_dict,
                          data_name='train',
                          missing_rate=None,
                          pre_points=None):
        if missing_rate is None:
            missing_rate = self.missing_rate
        if data_dict is None:
            return None
        if produce_intercoeffs:
            file_detail = data_name + '_' + str(int(missing_rate * 100) if missing_rate is not None else 0) + '_' + \
                str(pre_points if pre_points is not None else 0) + '_' + \
                (ode_time if ode_time is not None else 'Default') + '_' + \
                ('with_time' if x_data_with_time else 'without_time')
            coeffs_save_file = ub.Path('data') / dataset / 'coeffs_' + file_detail + '.pkl'
            if coeffs_save_file.exists and load_data:
                return load_pickle(coeffs_save_file)
        # TODO: curriculum learning for train data
        #TRANS: 1. From the partition of the input for the sampling, and will join the new dict mask, 50 points by default
        data = data_dict['x_data'].clone()
        device = get_device(data)
        mask = torch.ones_like(data, dtype=torch.bool,
                               device=device)  #TRANS: With bool type, the first to generate batch change again when the float value

        if missing_rate is not None:
            time_points = len(data_dict['x_time'][0, :, 0])
            missing_points = min(int(time_points * 0.9),
                                 int(time_points * missing_rate))
            #TRANS: For each slice, random time_points * missing_rate inactivation
            for i in range(data.shape[0]):
                missing_idx = sorted(
                    np.random.choice(np.arange(time_points),
                                     missing_points,
                                     replace=False))
                mask[i, missing_idx, :] = False

        #TRANS: Update the data and mask
        new_data_dict = {}
        new_data_dict['x_data'] = data.clone()
        new_data_dict['x_time'] = data_dict['x_time'].clone()
        new_data_dict['x_mask'] = mask.clone()

        #TRANS: 2. Use pre_points truncate the output
        data = data_dict['y_data'].clone()
        mask = torch.ones_like(data, dtype=torch.bool,
                               device=device)  #TRANS: Switch to a bool type, direct judgment

        if pre_points is not None:
            pre_points = min(pre_points, len(data_dict['y_time'][0, :, 0]))
            mask[:, pre_points:, :] = False

        new_data_dict['y_data'] = data.clone()
        new_data_dict['y_time'] = data_dict['y_time'].clone()
        new_data_dict['y_mask'] = mask.clone()

        if produce_intercoeffs:
            produce_and_save_intercoeffs(new_data_dict, coeffs_save_file)

        return _wrap_dataloader(new_data_dict, min(batch_size, new_data_dict['x_data'].shape[0]))

    def _produce_and_save_intercoeffs(self, data_dict, coeffs_save_file):
        #TRANS: Calculation and save the interpolation, the use of DataLoader save DataLoader
        data_dict['x_data'][~data_dict['x_mask']] = torch.nan
        data_dict['x_data'] = torchcde.natural_cubic_coeffs(
            data_dict['x_data'])
        dataloader = _wrap_dataloader(data_dict,
                                     batch_size=min(self.batch_size, data_dict['x_data'].shape[0]),
                                     shuffle=self.shuffle)
        save_items_to_pickle(dataloader, coeffs_save_file)

    def _wrap_dataloader(self, data_dict, **kwargs):
        #TRANS: Use the Dataloader
        dataset = torch.utils.data.TensorDataset(
            data_dict['x_time'], data_dict['x_data'], data_dict['x_mask'],
            data_dict['y_time'], data_dict['y_data'], data_dict['y_mask'])
        assert 'batch_size' in kwargs
        if 'shuffle' not in kwargs:
            kwargs['shuffle'] = True
        if 'drop_last' not in kwargs:
            kwargs['drop_last'] = True
        if 'num_workers' not in kwargs:
            kwargs['num_workers'] = 0
        kwargs['batch_size'] = min(kwargs['batch_size'], len(dataset))
        return torch.utils.data.DataLoader(dataset, **kwargs)


#TRANS: The definition and measurement model
def get_params_num(model):
    #TRANS: Filter receives two parameters, the first function, the second for the sequence, the sequence of each element passed as a parameter to a function for judgment, then return True or False, the final returns True, calculation and the number of elements in the new list
    model_parameters = filter(lambda p: p.requires_grad, model.parameters())
    n_params = sum([np.prod(p.shape) for p in model_parameters])
    kilo_params = n_params // 1000
    mod_params = n_params % 1000
    return str(kilo_params) + ' k ' + str(mod_params)


def create_net(input_dims,
               output_dims=None,
               n_units=100,
               hidden_layers=0,
               nonlinear=nn.Tanh,
               device=torch.device('cpu'),
               return_dim=False):
    if output_dims is None:
        output_dims = input_dims
    layers = [nn.Linear(input_dims, n_units)]

    for _ in range(hidden_layers):
        layers.append(nonlinear())
        layers.append(nn.Linear(n_units, n_units))

    layers.append(nonlinear())
    layers.append(nn.Linear(n_units, output_dims))
    #TRANS: In the function of passing, * trying to object to a tuple (variable parameter), * * trying to object to a dictionary (keyword arguments), both can be introduced to zero or any parameters
    if return_dim:
        return nn.Sequential(*layers).to(device), input_dims, output_dims
    else:
        return nn.Sequential(*layers).to(device)


def split_last_dim(data):
    #TRANS: Bisect tensor last dimension, in order to distinguish between the mean and STD
    last_dim = data.shape[-1]
    last_dim = last_dim // 2
    return data[..., :last_dim], data[..., last_dim:]


def init_network_weights(net, std=0.1):
    #TRANS: Initialize the gaussian distribution
    #TRANS: TODO: if use Xavier initialization?
    for m in net.modules():
        if isinstance(m, nn.Linear):
            nn.init.normal_(m.weight, mean=0, std=std)
            nn.init.constant_(m.bias, val=0)


def sample_standard_gaussian(mu, sigma):
    device = get_device(mu)

    d = torch.distributions.normal.Normal(
        torch.Tensor([0.]).to(device),
        torch.Tensor([1.]).to(device))
    r = d.sample(mu.shape).squeeze(-1)
    return r * sigma.float() + mu.float()


def linspace_vector(start, end, n_points, device=torch.device('cpu')):
    # start is either one value or a vector
    size = np.prod(start.shape)

    assert start.shape == end.shape
    if size == 1:
        # start and end are 1d-tensors
        res = torch.linspace(start, end, n_points, device=device)  #TRANS: Including end endpoint
    else:  #TRANS: Support the multi-dimensional linspace, for each dimension separately as a vector space
        # start and end are vectors
        res = torch.Tensor(device=device)
        for i in range(0, start.shape[0]):
            res = torch.cat((res, torch.linspace(start[i], end[i], n_points)),
                            0)
        res = torch.t(res.reshape(start.shape[0], n_points))
    return res


def time_sync_batch_dict(batch_list, ode_time=None):
    batch_dict = {}
    (batch_dict['x_time'], batch_dict['x_data'], batch_dict['x_mask'],
     batch_dict['y_time'], batch_dict['y_data'],
     batch_dict['y_mask']) = batch_list

    #TRANS: Time synchronization in the batch
    if ode_time is not None:
        if ode_time == 'Concat':
            concat_batch(batch_dict)
        elif ode_time == 'Mean':
            #TRANS: The average directly, reduced to 1 d form
            batch_dict['x_time'] = torch.mean(batch_dict['x_time'],
                                              dim=0).squeeze()
            batch_dict['y_time'] = torch.mean(batch_dict['y_time'],
                                              dim=0).squeeze()
        elif ode_time == 'No':
            #TRANS: Take points, starting from 0
            device = get_device(batch_dict['x_time'])
            dtype = batch_dict['x_time'].dtype
            batch_dict['x_time'] = torch.arange(
                start=0,
                end=batch_dict['x_time'].shape[1],
                dtype=dtype,
                device=device)
            batch_dict['y_time'] = torch.arange(
                start=0,
                end=batch_dict['y_time'].shape[1],
                dtype=dtype,
                device=device)
        else:
            raise NotImplementedError
    else:
        batch_dict['x_time'] = batch_dict['x_time'][0].squeeze()
        batch_dict['y_time'] = batch_dict['y_time'][0].squeeze()
    return batch_dict


def concat_batch(batch_dict):
    #TRANS: Whether you need batch_dict [' x_time] [0, 0, 0]!
    batch_size = batch_dict['x_data'].shape[0]
    #TRANS: Stitching plan 1, generated batch_size extension tensor, extremely time-consuming # # # # # # # # TODO: modify splicing
    _, time_points, x_dims = batch_dict['x_data'].shape
    device = get_device(batch_dict['x_data'])
    x_sync_time = torch.unique(batch_dict['x_time'][:, :, 0],
                               sorted=True).flatten()
    x_sync_time_points = x_sync_time.shape[0]
    x_sync_data = torch.zeros((batch_size, x_sync_time_points, x_dims),
                              device=device)
    x_sync_mask = torch.zeros_like(x_sync_data, dtype=torch.bool)
    for i in range(batch_size):
        cur = 0
        for j in range(x_sync_time_points):
            if cur >= time_points:
                break
            if x_sync_time[j] == batch_dict['x_time'][i, cur, 0]:
                x_sync_data[i, j, :] = batch_dict['x_data'][i, cur, :]
                x_sync_mask[i, j, :] = batch_dict['x_mask'][i, cur, :]
                cur += 1
    batch_dict['x_data'] = x_sync_data
    batch_dict['x_mask'] = x_sync_mask
    batch_dict['x_time'] = x_sync_time
    #TRANS: Whether you need batch_dict [' y_time] [0, 0, 0]!
    _, time_points, y_dims = batch_dict['y_data'].shape
    y_sync_time = torch.unique(batch_dict['y_time'][:, :, 0],
                               sorted=True).flatten()
    y_sync_time_points = y_sync_time.shape[0]
    y_sync_data = torch.zeros((batch_size, y_sync_time_points, y_dims),
                              device=device)
    y_sync_mask = torch.zeros_like(y_sync_data,
                                   dtype=torch.bool)  #TRANS: The y value, its mask is a Boolean value
    for i in range(batch_size):
        cur = 0
        for j in range(y_sync_time_points):
            if cur >= time_points:
                break
            if y_sync_time[j] == batch_dict['y_time'][i, cur, 0]:
                y_sync_data[i, j, :] = batch_dict['y_data'][i, cur, :]
                y_sync_mask[i, j, :] = batch_dict['y_mask'][i, cur, :]
                cur += 1
    batch_dict['y_data'] = y_sync_data
    batch_dict['y_mask'] = y_sync_mask
    batch_dict['y_time'] = y_sync_time
    if batch_size <= 1:
        return  #TRANS: Without stitching


#TRANS: Update the superparametric
def update_learning_rate(optimizer, decay_rate=0.999, lowest=1e-3):
    for param_group in optimizer.param_groups:
        lr = param_group['lr']
        lr = max(lr * decay_rate, lowest)
        param_group['lr'] = lr


def update_kl_coef(kl_first, epoch, wait_until_kl=10):
    if epoch < wait_until_kl:
        kl_coef = 0.
    else:
        kl_coef = kl_first * (1 - 0.99**(epoch - wait_until_kl))
    return kl_coef


#TRANS: Used for testing
def store_best_results(model, test_dataloader, kl_coef, experimentID,
                       res_files, ode_time, model_name):
    test_res = compute_loss_all_batches(model,
                                        test_dataloader,
                                        kl_coef,
                                        ode_time=ode_time)

    #TRANS: Check whether the corresponding value is updated, pay attention to each set of measurements metric_name requires a separate file
    res_dict, value_updated = update_metric_if_larger(
        res_files['best_results'], model_name, 'negative_MSE',
        -test_res['mse'].item())
    #TRANS: If after the update, change the best ID
    if value_updated:
        res_dict = update_value(res_files['best_experimentID'], model_name,
                                'negative_MSE', experimentID)

    return test_res, res_dict


def compute_loss_all_batches(model,
                             metric_dataloader,
                             kl_coef=1.,
                             ode_time='No'):
    res_dict_mean = {}
    res_dict_mean['loss'] = 0
    res_dict_mean['likelihood'] = 0
    res_dict_mean['mse'] = 0
    res_dict_mean['kl_coef'] = 0
    res_dict_mean['C_kl'] = 0
    res_dict_mean['C_std'] = 0

    n_test_batches = 0

    for metric_data_list in metric_dataloader:

        batch_dict = time_sync_batch_dict(metric_data_list, ode_time)
        results = model.compute_loss_one_batch(batch_dict, kl_coef=kl_coef)

        for key in res_dict_mean.keys():
            if key in results:
                var = results[key]
                if isinstance(var, torch.Tensor):
                    var = var.detach()
                res_dict_mean[key] += var

        n_test_batches += 1

    #TRANS: Take the average
    if n_test_batches > 1:
        for key, _ in res_dict_mean.items():
            res_dict_mean[key] = res_dict_mean[key] / n_test_batches

    return res_dict_mean


def update_metric_if_larger(results_file_name, model_name, metric_name, value):
    if not os.path.isfile(results_file_name) or os.path.getsize(
            results_file_name) == 0:  #TRANS: Does not exist or the file is empty
        with open(results_file_name, 'w') as f:
            f.write('model,' + metric_name)

    res_dict = pd.read_csv(results_file_name, sep=',')
    if metric_name not in res_dict.columns:
        res_dict[metric_name] = None

    value_updated = False

    if isinstance(value, torch.Tensor):
        #TRANS: Value = value. (CPU). Numpy # () to convert the amount to compare value type
        value = value.item()

    if (res_dict['model'] == model_name).any():
        prev_value = res_dict.loc[res_dict['model'] == model_name, metric_name]
        larger_than_prev = False
        if prev_value is not None and prev_value.item() is not None:
            larger_than_prev = float(prev_value.item()) < value
        if (prev_value.item() is
                None) or math.isnan(prev_value) or larger_than_prev:
            res_dict.loc[res_dict['model'] == model_name,
                         metric_name] = float(value)  #TRANS: Update the corresponding form
            value_updated = True
    else:  #TRANS: Add the corresponding model results
        res_dict = pd.concat([
            res_dict,
            pd.DataFrame({
                'model': [model_name],
                metric_name: [value]
            })
        ],
                             ignore_index=True)
        value_updated = True

    with open(results_file_name, 'w', newline='') as csvfile:  #TRANS: Rewrite the corresponding file
        res_dict.to_csv(csvfile, index=False)
    return res_dict, value_updated


def update_value(results_file_name, model_name, metric_name, value):
    if not os.path.isfile(results_file_name) or os.path.getsize(
            results_file_name) == 0:
        with open(results_file_name, 'w') as f:
            f.write('model,' + metric_name)

    res_dict = pd.read_csv(results_file_name, sep=',')
    if metric_name not in res_dict.columns:
        res_dict[metric_name] = None

    if (res_dict['model'] == model_name).any():  #TRANS: Is updated directly, do not need to size
        res_dict.loc[res_dict['model'] == model_name, metric_name] = value
    else:
        res_dict = pd.concat([
            res_dict,
            pd.DataFrame({
                'model': [model_name],
                metric_name: [value]
            })
        ],
                             ignore_index=True)

    with open(results_file_name, 'w', newline='') as csvfile:
        res_dict.to_csv(csvfile, index=False)
    return res_dict


def normalize_data_minmax(data):
    #TRANS: Auxiliary processing data
    data_min = torch.min(data, 0).values
    data_max = torch.max(data, 0).values
    numerator = data - data_min
    denominator = data_max - data_min
    norm_data = numerator / (denominator + 1e-7)  #TRANS: Join the decimal avoid divided by zero
    return norm_data, data_min, data_max


#TRANS: Save the checkpoint and results
def get_logger_and_save(model,
                        experimentID,
                        dataset_str,
                        experiment_str,
                        save_log=False,
                        save_fig_per_test=0,
                        save_ckpt=False,
                        save_res_for_epochs=None):

    if save_log:
        log_file = ub.Path('log').ensuredir() / experiment_str + '.log'
    else:
        log_file = None
    res_files = {
        'best_results': 'log/best_results_' + dataset_str + '.csv',
        'best_experimentID': 'log/best_experimentID_' + dataset_str + '.csv'
    }

    if save_fig_per_test > 0:
        fig_saveat = ub.Path('fig').ensuredir() / experiment_str
    else:
        fig_saveat = None

    if save_ckpt:
        ckpt_saveat = ub.Path('ckpt').ensuredir() / experiment_str
    else:
        ckpt_saveat = None

    print('ExperimentID: ' + experimentID)  #TRANS: See this print experiment has produced results file
    #TRANS: Message, warning will be output to the console and record to a file, info not output to the console
    logger = get_logger(log_file)
    note = ub.paragraph('''TRANS: 

拱门,解算器,dset、x_pts y_pts missing_pct,时间
{experiment_str}

“”)
logger.warning(注意)
记录器。
警告(“所有的参数都是:' + cf.log ())
#训练结果记录器
如果save_res_for_epochs不是没有和save_log:
train_res_csv = save_csv (log_saveat,“火车”)
val_res_csv = save_csv (log_saveat“val”)
其他:
train_res_csv =没有
val_res_csv =没有
返回日志记录器,res_files train_res_csv val_res_csv, fig_saveat ckpt_saveat


def get_logger (log_file = None):
记录器= logging.getLogger ()
logger.setLevel (logging.INFO)
如果log_file不是没有:
info_file_handler = logging.FileHandler (log_file)
info_file_handler.setLevel (logging.INFO)
logger.addHandler (info_file_handler)
console_handler = logging.StreamHandler ()
console_handler.setLevel (logging.WARNING)
logger.addHandler (console_handler)

返回日志记录器


def save_checkpoint (ckpt_saveat、州* * kwargs):
如果没有或不os.path.isdir ckpt_saveat (ckpt_saveat):
返回
如果kwargs“时代”:
文件名= os.path.join (ckpt_saveat,
“检查点——% 04 d.pth ' % kwargs['时代'])
elif kwargs:“名字”
文件名= os.path。
加入(ckpt_saveat ' % s.pth ' % kwargs['名字'])
其他:
提高ValueError
火炬。
文件名保存(状态)


def load_checkpoint (ckpt_saveat模型= None):
如果不是os.path.exists (ckpt_saveat):
提高异常(“检查点”+ ckpt_saveat +“不存在。”)
如果os.path.isdir (ckpt_saveat):
ckpt_saveat = os.path。
加入(ckpt_saveat SaveAll.pth)
检查点= torch.load (ckpt_saveat)
如果没有没有一个模型:#只获取模型对应状态
state_dict =检查点
model_dict = model.state_dict ()
# 1。
过滤掉不必要的钥匙
state_dict = {k: k v, v在state_dict.items()如果k model_dict}
# 2。
dict覆盖现有条目的状态
model_dict.update (state_dict)
# 3。
加载新的状态dict类型
model.load_state_dict (state_dict)
# model.to(设备)
回归模型
其他:
#模型=检查点(“模型”),(设备)
#优化器=检查点(“优化”)
#调度器=检查点(“调度”)
# pre_points =检查点(“pre_points”)
# experimentID =检查点(“ID”)
返回检查点


类save_csv(对象):
“结果写入csv
'''

    def __init__(self, csv_saveat, data_type='train'):
        super(save_csv, self).__init__()
        self.csv_saveat = csv_saveat  #TRANS: And the log in a folder
        self.data_type = data_type
        self.filepath = self.csv_saveat + '/' + self.data_type + '_' + str(
            0) + '.csv'
        self.no_head = True

    def write(self, res, epoch):
        row = [
            str(metric.item())
            if isinstance(metric, torch.Tensor) else str(metric)
            for metric in res.values()
        ]
        row.append(str(epoch))
        with open(self.filepath, 'a', newline='') as f:
            csv_writer = csv.writer(f)
            if self.no_head:
                head = list(res.keys())
                head.append('epoch')
                csv_writer.writerow(head)
                self.no_head = False  #TRANS: Just write a header
            csv_writer.writerow(row)
        new_filepath = self.csv_saveat + '/' + self.data_type + '_' + str(
            epoch) + '.csv'
        os.rename(self.filepath, new_filepath)
        self.filepath = new_filepath  #TRANS: Update the epoch number information


#TRANS: Pickled version
def save_items_to_pickle(data, filename):
    with open(filename, 'wb') as pkl_file:
        pickle.dump(data, pkl_file)


def load_pickle(filename):
    with open(filename, 'rb') as pkl_file:
        filecontent = pickle.load(pkl_file)
    return filecontent


def get_items_from_pickle(filename, item_name_list):
    pickle_items = load_pickle(filename)
    items = {}
    for item_name in item_name_list:
        if item_name in pickle_items:
            items[item_name] = pickle_items[item_name]
    return items
