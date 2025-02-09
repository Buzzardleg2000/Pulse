# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pathlib import Path
import pandas as pd
import numpy as np

def read_csv_into_df(csv_filename: Path, replace_slashes: bool=False, **kwargs):
    df = pd.read_csv(csv_filename, **kwargs)
    for column in df.columns[1:]:
        # Convert any strings to NaN
        df[column] = pd.to_numeric(df[column], downcast='float', errors='coerce')
        # Replace slashes in units string
        if replace_slashes:
            df.rename(columns={column: column.replace("/", "_Per_")}, inplace=True)

    return df


def compute_means(csv_filename: Path, headers: [str], start_row=0, end_row=-1):
    means = []

    # Load up the csv file, pull the headers, and compute the mean of values from start to end
    df = read_csv_into_df(csv_filename).loc[start_row:end_row]
    means = df[headers].mean().values.tolist()

    return means


def remove_empty_cols(df: pd.DataFrame):
    drop_cols = []
    for c in df.columns:
        if df[c].replace(r'^-1\.\$$', np.nan, regex=True).isna().all():
            drop_cols.append(c)
    if drop_cols:
        return df.drop(columns=drop_cols)
    return df
