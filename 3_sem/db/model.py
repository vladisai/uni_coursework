class RawLogEntry:

    def __init__(self, time, driver_name, message):
        self.time = time
        self.driver_name = driver_name
        self.message = message

    def __str__(self):
        return str(self.id) + ' time(' + str(self.time) + ') driver(' + self.driver_name + ') message(' + self.message + ')'

class LogEntry:

    def __init__(self, log_id, time, driver_id, message):
        self.log_id = log_id
        self.time = time
        self.driver_id = driver_id
        self.message = message

class DriverEntry:

    def __init__(self, driver_id, driver_name):
        self.driver_id = driver_id
        self.driver_name = driver_name
