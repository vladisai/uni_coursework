from model import LogEntry, DriverEntry

class LogsCollection:

    def __init__(self):
        self.dict = {}
        self.driver_counter = 1
        self.log_counter = 1
        self.drivers = []
        self.logs = []

    def addEntry(self, entry):
        if self.dict.has_key(entry.driver_name):
            self.dict[entry.driver_name] = self.counter
            self.drivers.append(DriverEntry(self.driver_counter, entry.driver_name))
            self.driver_counter += 1
        self.logs.append(LogEntry(self.log_counter, entry.time, self.dict[entry.driver_name], entry.message))
