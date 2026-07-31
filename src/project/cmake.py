class CMake(object):
    def __init__(self, version: str, c_std: int, cxx_std: int) -> None:
        self.version: str = version
        self.c_std: int = c_std
        self.cxx_std: int = cxx_std