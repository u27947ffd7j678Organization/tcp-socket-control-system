"""Helpers for parsing TCP server status responses."""

from __future__ import annotations


def parse_status_response(response: str) -> dict[str, str] | None:
    """Parse a STATUS response such as: STATUS STATE=RUN TEMP=25.4 HUMI=52.1."""
    parts = response.strip().split()
    if not parts or parts[0] != "STATUS":
        return None

    values: dict[str, str] = {}
    for part in parts[1:]:
        if "=" not in part:
            return None
        key, value = part.split("=", 1)
        if not key or not value:
            return None
        values[key] = value

    required_keys = {"STATE", "TEMP", "HUMI"}
    if not required_keys.issubset(values):
        return None

    if values["STATE"] not in {"RUN", "STOP"}:
        return None

    try:
        float(values["TEMP"])
        float(values["HUMI"])
    except ValueError:
        return None

    return values

