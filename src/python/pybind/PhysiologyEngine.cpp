/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEngine.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void PhysiologyEngine(py::module &m)
{
  py::class_<PulseEngineThunk>(m, "Engine", py::buffer_protocol())
    .def(py::init<>())
    .def("serialize_from_file", &PulseEngineThunk::SerializeFromFile)
    .def("serialize_to_file", &PulseEngineThunk::SerializeToFile)
    .def("serialize_from_string", &PulseEngineThunk::SerializeFromString)
    .def("serialize_to_string", &PulseEngineThunk::SerializeToString)
    .def("initialize_engine", &PulseEngineThunk::InitializeEngine)
    .def("get_initial_patient", &PulseEngineThunk::GetInitialPatient)
    .def("log_to_console", &PulseEngineThunk::LogToConsole)
    .def("keep_log_messages", &PulseEngineThunk::KeepLogMessages)
    .def("set_log_filename", &PulseEngineThunk::SetLogFilename)
    .def("pull_log_messages", &PulseEngineThunk::PullLogMessages)
    .def("keep_event_changes", &PulseEngineThunk::KeepEventChanges)
    .def("pull_events", &PulseEngineThunk::PullEvents)
    .def("pull_active_events", &PulseEngineThunk::PullActiveEvents)
    .def("process_actions", &PulseEngineThunk::ProcessActions)
    .def("advance_timestep", &PulseEngineThunk::AdvanceTimeStep)
    .def("get_timestep", &PulseEngineThunk::GetTimeStep)
    .def("pull_data", [](PulseEngineThunk& t) -> py::array {
       double* r = t.PullDataPtr();
       // This is a copy, but it's pretty small
       // If we need to, we can not copy, and send a buffer info description
       return py::array(t.DataLength(), r);
     })
    ;
}