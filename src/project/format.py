# Builtin Imports
from typing import Final
import re

_WORDS : Final[re.Pattern[str]] = re.compile(r"[A-Z]+(?=[A-Z][a-z])|[A-Z]?[a-z]+|[A-Z]+|[0-9]+")

def to_snake_case(string: str) -> str: return "_".join(_WORDS.findall(string)).lower()

def to_screaming_case(string: str) -> str: return "_".join(_WORDS.findall(string)).upper()

def to_pascal_case(string: str) -> str: return "".join(map(str.capitalize, _WORDS.findall(string)))
