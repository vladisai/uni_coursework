import sqlite3
import os.path
import os

class FileAlreadyExists(Exception):
    pass

class DBLoader:
    
    def saveToDatabase(self):
        pass

class DBLoaderSqlite3(DBLoader):

    def __init__(self, dbname):

    def checkIfFileExists(name):
        return os.path.isfile(name) 

    def init_database(name):
        if checkIfFileExists(name):
            raise FileAlreadyExists() 

        connection = sqlite3.connect(name)

        createLogsTableCommand = """CREATE TABLE Logs
                                    (
                                        log_id INTEGER PRIMARY KEY,
                                        machine_name TEXT
                                    );""";

        createDriversTableCommand = """CREATE TABLE Drivers 
                                        (
                                            driver_id INTEGER PRIMARY KEY, 
                                            name TEXT
                                        );"""

        createLogEntryTableCommand = """CREATE TABLE LogEntry 
                                        (
                                            message_id INTEGER PRIMARY KEY,
                                            message TEXT,
                                            date INTEGER,
                                            driver_id INTEGER,
                                                FOREIGN KEY (driver_id) REFERENCES Drivers(driver_id)
                                        );"""

        c = connection.cursor() 

        c.execute(createDriversTableCommand)
        c.execute(createLogEntryTableCommand)
        c.execute(createLogsTableCommand)

        connection.commit()
        connection.close()

    def saveToDatabase(self):
