class Language(object):
    def __init__(self, name: str, std: int, hdr_ext: str, src_ext: str) -> None:
        self.name: str = name
        self.std: int = std
        self.hdr_ext: str = hdr_ext
        self.src_ext: str = src_ext

    def __str__(self) -> str: return self.name
