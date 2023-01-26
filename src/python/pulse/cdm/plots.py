# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import pandas as pd

from enum import Enum
from typing import Tuple

from pulse.cdm.scalars import SEScalar, SEScalarLength

class SEImageProperties():
    __slots__ = ["_file_format", "_height_inch", "_width_inch", "_dpi"]

    def __init__(self):
        self._file_format = ".png"
        self._height_inch = 2
        self._width_inch = 6
        self._dpi = 100

    def get_file_format(self):
        return self._file_format
    def set_file_format(self, fmt: str):
        self._file_format = fmt

    def get_height_inch(self):
        return self._height_inch
    def set_height_inch(self, h: float):
        self._height_inch = h

    def get_width_inch(self):
        return self._width_inch
    def set_width_inch(self, w: float):
        self._width_inch = w

    def get_dpi(self):
        return self._dpi
    def set_dpi(self, dpi: float):
        self._dpi = dpi


class SEBounds():
    __slots__ = ["_lower_bound", "_upper_bound"]

    def __init__(self):
        self._lower_bound = None
        self._upper_bound = None

    def get_lower_bound(self):
        if self._lower_bound is None:
            self._lower_bound = SEScalar()
        return self._lower_bound
    def set_lower_bound(self, bound: float):
        self._lower_bound = bound
    def has_lower_bound(self):
        return self._lower_bound is not None
    def invalidate_lower_bound(self):
        self._lower_bound = None

    def get_upper_bound(self):
        if self._upper_bound is None:
            self._upper_bound = SEScalar()
        return self._upper_bound
    def set_upper_bound(self, bound: float):
        self._upper_bound = bound
    def has_upper_bound(self):
        return self._upper_bound is not None
    def invalidate_upper_bound(self):
        self._upper_bound = None


class ePercentageOfBaselineMode(Enum):
    Off = 0
    All = 1
    OnlyX = 2
    OnlyY = 3
class eTickStyle(Enum):
    Scientific = 0
    Plain = 1
class SEPlotSettings():
    __slots__ = ["_font_size", "_gridlines", "_image_properties",
                 "_legend_font_size", "_log_axis", "_output_filename",
                 "_output_path_override", "_percent_of_baseline_mode",
                 "_remove_legends", "_sci_limits", "_tick_style", "_title",
                 "_x1_label", "_x1_bounds", "_y1_label", "_y1_bounds"]

    def __init__(self):
        self._font_size = 22
        self._gridlines = False
        self._image_properties = SEImageProperties()
        self._legend_font_size = 15
        self._log_axis = False
        self._output_filename = ""
        self._output_path_override = "../verification/Plots/"
        self._percent_of_baseline_mode = ePercentageOfBaselineMode.Off
        self._remove_legends = False
        self._sci_limits = None
        self._tick_style = eTickStyle.Scientific
        self._title = None
        self._x1_label = None
        self._x1_bounds = SEBounds()
        self._y1_label = None
        self._y1_bounds = SEBounds()

    def get_font_size(self):
        return self._font_size
    def set_font_size(self, font_size: float):
        self._font_size = font_size

    def get_gridlines(self):
        return self._gridlines
    def set_gridlines(self, gridlines: bool):
        self._gridlines = gridlines

    def get_image_properties(self):
        return self._image_properties
    def set_image_properties(self, image_props: SEImageProperties):
        self._image_properties = image_props

    def get_legend_font_size(self):
        return self._legend_font_size
    def set_legend_font_size(self, font_size: float):
        self._legend_font_size = font_size

    def get_log_axis(self):
        return self._log_axis
    def set_log_axis(self, log_axis: bool):
        self._log_axis = log_axis

    def get_output_filename(self):
        return self._output_filename
    def set_output_filename(self, output_filename: str):
        self._output_filename = output_filename

    def get_output_path_override(self):
        return self._output_path_override
    def set_output_path_override(self, output_path: str):
        self._output_path_override = output_path
    def has_output_path_override(self):
        return self._output_path_override is not None
    def invalidate_output_path_override(self):
        self._output_path_override = None

    def get_percent_of_baseline_mode(self):
        return self._percent_of_baseline_mode
    def set_percent_of_baseline_mode(self, mode: ePercentageOfBaselineMode):
        self._percent_of_baseline_mode = mode

    def get_remove_legends(self):
        return self._remove_legends
    def set_remove_legends(self, remove_legends: bool):
        self._remove_legends = remove_legends

    def get_sci_limits(self):
        return self._sci_limits
    def set_sci_limits(self, limits: Tuple[int, int]):
        self._sci_limits = limits
    def has_sci_limits(self):
        return self._sci_limits is not None

    def get_tick_style(self):
        return self._tick_style
    def set_tick_style(self, style: eTickStyle):
        self._tick_style = style

    def get_title(self):
        return self._title
    def set_title(self, title: str):
        self._title = title
    def has_title(self):
        return self._title is not None
    def invalidate_title(self):
        self._title = None

    def get_x1_label(self):
        return self._x1_label
    def set_x1_label(self, label: str):
        self._x1_label = label
    def has_x1_label(self):
        return self._x1_label is not None
    def invalidate_x1_label(self):
        self._x1_label = None

    def get_x1_bounds(self):
        return self._x1_bounds
    def set_x1_bounds(self, bounds: SEBounds):
        self._x1_bounds = bounds

    def get_y1_label(self):
        return self._y1_label
    def set_y1_label(self, label: str):
        self._y1_label = label
    def has_y1_label(self):
        return self._y1_label is not None
    def invalidate_y1_label(self):
        self._y1_label = None

    def get_y1_bounds(self):
        return self._y1_bounds
    def set_y1_bounds(self, bounds: SEBounds):
        self._y1_bounds = bounds


class SEPlotSource():
    __slots__ = ["_csv_data", "_df", "_fill_area", "_label", "_line_format",
                 "_begin_row", "_end_row"]

    def __init__(self):
        self._csv_data = None
        self._df = None
        self._fill_area = False
        self._label = None
        self._line_format = "-k"
        self._begin_row = None
        self._end_row = None

    def get_csv_data(self):
        return self._csv_data
    def set_csv_data(self, csv_data: str):
        self._csv_data = csv_data
    def has_csv_data(self):
        return self._csv_data is not None
    def invalidate_csv_data(self):
        self._csv_data = None
        self._df = None

    def _read_csv_into_df(self, replace_slashes=True):
        if not self.has_csv_data():
            raise Exception("No CSV data provided")
        self._df = pd.read_csv(self._csv_data)
        for column in self._df.columns[1:]:
            # Convert any strings to NaN
            self._df[column] = pd.to_numeric(self._df[column], errors='coerce')
            # Replace slashes in units string
            if replace_slashes:
                self._df.rename(columns={column: column.replace("/", "_Per_")}, inplace=True)
    def get_data_frame(self):
        if self._df is None:
            self._read_csv_into_df(replace_slashes=False)
        return self._df.loc[self._begin_row:self._end_row]
    def invalidate_data_frame(self):
        self._df = None

    def get_fill_area(self):
        return self._fill_area
    def set_fill_area(self, fill: bool):
        self._fill_area = fill

    def get_label(self):
        return self._label
    def set_label(self, label: str):
        self._label = label
    def has_label(self):
        return self._label is not None
    def invalidate_label(self):
        self._label = None

    def get_line_format(self):
        return self._line_format
    def set_line_format(self, fmt: str):
        self._line_format = fmt

    def get_begin_row(self):
        return self._begin_row
    def set_begin_row(self, row: int):
        self._begin_row = row
    def has_begin_row(self):
        return self._begin_row is not None

    def get_end_row(self):
        return self._end_row
    def set_end_row(self, row: int):
        self._end_row = row
    def has_end_row(self):
        return self._end_row is not None

class SEPlotter():
    __slots__ = ["_plot_settings"]

    def __init__(self):
        self._plot_settings = None

    def get_plot_settings(self):
        if self._plot_settings is None:
            self._plot_settings = SEPlotSettings()
        return self._plot_settings
    def set_plot_settings(self, settings: SEPlotSettings):
        self._plot_settings = settings
    def has_plot_settings(self):
        return self._plot_settings is not None

class SESeries():
    __slots__ = ["_header", "_output_filename", "_title", "_label"]

    def __init__(self):
        self._header = None
        self._output_filename = None
        self._title = None
        self._label = None

    def get_header(self):
        return self._header
    def set_header(self, header: str):
        self._header = header
    def has_header(self):
        return self._header is not None
    def invalidate_header(self):
        self._header = None

    def get_output_filename(self):
        return self._output_filename
    def set_output_filename(self, output_filename: str):
        self._output_filename = output_filename
    def has_output_filename(self):
        return self._output_filename is not None
    def invalidate_output_filename(self):
        self._output_filename = None

    def get_title(self):
        return self._title
    def set_title(self, title: str):
        self._title = title
    def has_title(self):
        return self._title is not None
    def invalidate_title(self):
        self._title = None

    def get_label(self):
        return self._label
    def set_label(self, label: str):
        self._label = label
    def has_label(self):
        return self._label is not None
    def invalidate_label(self):
        self._label = None

class SETimeSeriesPlotter(SEPlotter):
    __slots__ = ["_plot_source", "_series"]

    def __init__(self):
        super().__init__()

        self._plot_source = None
        self._series = []

    def get_plot_source(self):
        if self._plot_source is None:
            self._plot_source = SEPlotSource()
        return self._plot_source
    def set_plot_source(self, plot_source: SEPlotSource):
        self._plot_source = plot_source
    def has_plot_source(self):
        return self._plot_source is not None

    def get_series(self):
        return self._series
    def add_series(self, series: SESeries):
        self._series.append(series)
    def has_series(self):
        return len(self._series) > 0
    def invalidate_series(self):
        self._series = []

class SERelation():
    __slots__ = ["_header_x", "_header_y", "_output_filename", "_title",
                 "_label_x", "_label_y"]

    def __init__(self):
        self._header_x = None
        self._header_y = None
        self._output_filename = None
        self._title = None
        self._label_x = None
        self._label_y = None

    def get_header_x(self):
        return self._header_x
    def set_header_x(self, header: str):
        self._header_x = header
    def has_header_x(self):
        return self._header_x is not None
    def invalidate_header_x(self):
        self._header_x = None

    def get_header_y(self):
        return self._header_y
    def set_header_y(self, header: str):
        self._header_y = header
    def has_header_y(self):
        return self._header_y is not None
    def invalidate_header_y(self):
        self._header_y = None

    def get_output_filename(self):
        return self._output_filename
    def set_output_filename(self, output_filename: str):
        self._output_filename = output_filename
    def has_output_filename(self):
        return self._output_filename is not None
    def invalidate_output_filename(self):
        self._output_filename = None

    def get_title(self):
        return self._title
    def set_title(self, title: str):
        self._title = title
    def has_title(self):
        return self._title is not None
    def invalidate_title(self):
        self._title = None

    def get_label_x(self):
        return self._label_x
    def set_label_x(self, label: str):
        self._label_x = label
    def has_label_x(self):
        return self._label_x is not None
    def invalidate_label_x(self):
        self._label_x = None

    def get_label_y(self):
        return self._label_y
    def set_label_y(self, label: str):
        self._label_y = label
    def has_label_y(self):
        return self._label_y is not None
    def invalidate_label_y(self):
        self._label_y = None

class SERelationalPlotter(SEPlotter):
    __slots__ = ["_plot_source", "_relationships"]

    def __init__(self):
        super().__init__()

        self._plot_source = None
        self._relationships = []

    def get_plot_source(self):
        if self._plot_source is None:
            self._plot_source = SEPlotSource()
        return self._plot_source
    def set_plot_source(self, plot_source: SEPlotSource):
        self._plot_source = plot_source
    def has_plot_source(self):
        return self._plot_source is not None

    def get_relationships(self):
        return self._relationships
    def add_relation(self, relation: SERelation):
        self._relationships.append(relation)
    def has_relationships(self):
        return len(self._relationships) > 0
    def invalidate_relationships(self):
        self._relationships = []
