from datetime import datetime
import os

def get_modified_date_timestamp(path):
    return int(os.path.getmtime(path))

def get_file_modified_date(path: str) -> str:
    """
    Return the last modified date of a file as 'YY/MM/DD'.
    """
    timestamp = os.path.getmtime(path)
    dt = datetime.fromtimestamp(timestamp)
    return dt.strftime("%y/%m/%d")