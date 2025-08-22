import tempfile
from pathlib import Path

import pydicom
from pydicom.dataset import Dataset, FileDataset, FileMetaDataset
from pydicom.uid import generate_uid, UID, ExplicitVRLittleEndian
import datetime


class DicomManager:
    def __init__(self):
        pass

    def new(self, p_name, p_id):
        ds = Dataset()
        ds.PatientName = p_name
        ds.PatientID = p_id
        # Set creation date/time
        dt = datetime.datetime.now()
        ds.ContentDate = dt.strftime("%Y%m%d")
        ds.ContentTime = dt.strftime("%H%M%S.%f")  # long format with micro seconds

        print("Setting file meta information...")
        # Populate required values for file meta information
        file_meta = FileMetaDataset()
        file_meta.MediaStorageSOPClassUID = UID("1.2.840.10008.5.1.4.1.1.2")
        file_meta.MediaStorageSOPInstanceUID = UID("1.2.3")
        file_meta.ImplementationClassUID = UID("1.2.3.4")
        file_meta.TransferSyntaxUID = ExplicitVRLittleEndian

        # Add the file meta information
        ds.file_meta = file_meta

        path = Path(tempfile.NamedTemporaryFile(suffix=".dcm").name)
        print(f"Writing dataset to: {path}")
        ds.save_as(path, enforce_file_format=True)

        # reopen the data just for checking
        print(f"Load dataset from: {path} ...")
        ds = pydicom.dcmread(path)

        ds.add_new()
