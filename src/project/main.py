# Builtin Imports
from typing import Final
import sys
import traceback

# Pip Imports
import click

# Local Imports
from project.project import Project
from project.language import Language

CMAKE_VERSION: Final[str] = "4.3.0"
LANGUAGES: Final[dict[str, Language]] = {
    "C": Language(name="C", std=23, hdr_ext=".h", src_ext=".c"),
    "C++": Language(name="C++", std=26, hdr_ext=".hpp", src_ext=".cpp")
}

@click.command("create-project")
@click.argument("project_name", required=True, type=click.STRING)
@click.argument("project_language", required=True, type=click.Choice(["C", "C++"]))
@click.argument("project_type", required=True, type=click.Choice(["Executable", "Static Library", "Shared Library", "Interface Library"]))
@click.argument("project_author", required=True, type=click.STRING)
@click.argument("project_namespace", required=False, type=click.STRING, default="")
@click.argument("project_version", required=False, type=click.STRING, default="0.1.0")
@click.argument("project_description", required=False, type=click.STRING, default="")
def main(project_name: str,
         project_language: str,
         project_type: str,
         project_author: str,
         project_namespace: str,
         project_version: str,
         project_description: str) -> int: # raises SystemExit
    try:
        # Create a new instance of Project
        project: Project = Project(project_name,
                                   LANGUAGES[project_language],
                                   project_type,
                                   project_author,
                                   project_namespace,
                                   project_version,
                                   project_description)

        # Render the project
        project.render(CMAKE_VERSION)

        return 0
    except Exception as e:
        traceback.print_exception(e)
        raise SystemExit(1)

if __name__ == "__main__":
    sys.exit(main())