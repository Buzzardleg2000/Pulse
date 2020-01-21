import PyPulse
import numpy
from enum import Enum

class SerializationFormat(Enum):
    BINARY = 0
    JSON = 1

class PulsePhysiologyEngine:
    __slots__ = ['__pulse', "results"]

    def __init__(self, log_file="", write_to_console=True, data_root="."):
        self.results = {}
        self.__pulse = PyPulse.Engine(log_file, write_to_console, data_root)

    def serialize_from_file(self, state_file, data_requests, format, start_time):
        # Process requests and setup our results structure
        self.process_requests(data_requests)
        fmt = PyPulse.serialization_format.json
        if format == SerializationFormat.BINARY:
            fmt = PyPulse.serialization_format.binary
        return self.__pulse.serialize_from_file(state_file, "", fmt, start_time)

    def advance_time(self):
        return self.__pulse.advance_timestep()

    def pull_data(self):
        values = self.__pulse.pull_data()
        i=0
        for key, value in self.results.items():
            self.results[key] = values[i]
            i+=1
        return self.results

    def process_requests(self, data_requests):
        if data_requests is None:
            self.results["SimulationTime(s)"]=0
            self.results["Lead3ElectricPotential(mV)"]=0
            self.results["HeartRate(bpm)"]=0
            self.results["ArterialPressure(mmHg)"]=0
            self.results["MeanArterialPressure(mmHg)"]=0
            self.results["SystolicArterialPressure(mmHg)"]=0
            self.results["DiastolicArterialPressure(mmHg)"]=0
            self.results["OxygenSaturation"]=0
            self.results["EndTidalCarbonDioxidePressure(mmHg)"]=0
            self.results["RespirationRate(bpm)"]=0
            self.results["CoreTemperature(C)"]=0
            self.results["CarinaCO2PartialPressure(mmHg)"]=0
            self.results["BloodVolume(mL)"]=0


