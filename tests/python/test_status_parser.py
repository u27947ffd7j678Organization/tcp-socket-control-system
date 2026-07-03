import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
GUI_CLIENT_DIR = PROJECT_ROOT / "client" / "python_gui"
sys.path.insert(0, str(GUI_CLIENT_DIR))

from status_parser import parse_status_response


def test_parse_status_response_run():
    assert parse_status_response("STATUS STATE=RUN TEMP=25.4 HUMI=52.1") == {
        "STATE": "RUN",
        "TEMP": "25.4",
        "HUMI": "52.1",
    }


def test_parse_status_response_stop():
    assert parse_status_response("STATUS STATE=STOP TEMP=25.4 HUMI=52.1") == {
        "STATE": "STOP",
        "TEMP": "25.4",
        "HUMI": "52.1",
    }


def test_parse_status_response_rejects_non_status_responses():
    assert parse_status_response("PONG") is None
    assert parse_status_response("OK START") is None


def test_parse_status_response_rejects_missing_state():
    assert parse_status_response("STATUS TEMP=25.4 HUMI=52.1") is None


def test_parse_status_response_rejects_invalid_temperature():
    assert parse_status_response("STATUS STATE=RUN TEMP=abc HUMI=52.1") is None

