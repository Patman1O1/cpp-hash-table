# Builtin Imports
import os
from pathlib import Path
import shutil

# Pip Imports
from jinja2 import Environment, FileSystemLoader

# Local Imports
from project import language
from project.format import *
from project.language import Language

class Project(object):
    ROOT: Final[Path] = Path(__file__).resolve().parents[2]

    def __init__(self,
                 project_name: str,
                 project_language: Language,
                 project_type: str,
                 project_author: str,
                 project_namespace: str = "",
                 project_version: str = "0.1.0",
                 project_description: str = "") -> None: # raises ValueError
        self.name: str = to_snake_case(project_name)
        self.package_name: str = to_pascal_case(project_name)
        self.language: Language = project_language
        self.type: str = project_type
        self.author: str = project_author
        self.namespace: str = to_snake_case(project_namespace) if project_namespace != "" else self.name
        self.version: str = project_version
        self.description: str = project_description

    @staticmethod
    def _create_env(search_path: Path) -> Environment:
        env: Environment = Environment(
            loader=FileSystemLoader(search_path),
            keep_trailing_newline=True,
            trim_blocks=True,
            lstrip_blocks=True,
        )

        env.filters["to_screaming_case"] = to_screaming_case
        env.filters["to_pascal_case"] = to_pascal_case

        return env

    def render(self, cmake_version: str) -> None:
        template_dir: Path = Project.ROOT/"template"
        env = Project._create_env(template_dir)

        skip = {
            "Executable": {"include", "test_package"},
            "Interface Library": {"src"},
        }.get(self.type, set())

        for path in template_dir.rglob("*.j2"):
            rel: Path = path.relative_to(template_dir)
            if rel.parts[0] in skip:
                continue

            # Interpolate {{ }} in every path segment, then drop the .j2 suffix
            parts: list[str] = [env.from_string(p).render(project=self,
                                                          language=self.language,
                                                          cmake_version=cmake_version) for p in rel.parts]
            parts[-1] = parts[-1].removesuffix(".j2")
            dest: Path = Project.ROOT.joinpath(*parts)

            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(
                env.get_template(rel.as_posix()).render(project=self,
                                                        language=self.language,
                                                        cmake_version=cmake_version), encoding="utf-8",
            )

        # Write the project description to the README.md file
        with open(Project.ROOT/"README.md", "w", encoding="utf-8") as readme_md:
            readme_md.write(f"# {self.name}\n\n{self.description}\n")

        # Remove template/
        shutil.rmtree(template_dir)

        # Remove .github/
        shutil.rmtree(Project.ROOT/".github")

        if self.type == "Executable":
            # Remove include/ if the project type is an Executable
            shutil.rmtree(Project.ROOT/"include")
        elif self.type == "Interface Library":
            # Remove src/ and export.h/.hpp if the project type is an Interface Library
            shutil.rmtree(Project.ROOT/"src")
            os.unlink(Project.ROOT/"include"/f"{self.namespace}"/f"export{self.language.hdr_ext}")

        # Remove pyproject.toml
        os.unlink(Project.ROOT/"pyproject.toml")
