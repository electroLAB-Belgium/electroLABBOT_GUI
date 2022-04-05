""" Compile the recipe for the electroLABBOT tools."""
import locale
import shutil
import subprocess
import time


def generate_version_information():
    """ Generate the version token and the date taken for this repository. """
    # Get repo information
    try:
        import git
        repo = git.Repo(search_parent_directories=True)
        url = repo.remotes.origin.url.split('.git')[0]
        sha = repo.head.object.hexsha
    except Exception:
        sha = 'none'
        url = ('https://github.com/vincent-stragier/')

    date = time.strftime("%Y-%m-%dT%H:%M:%S%z", time.gmtime())

    return date, sha, url


def run_command(command):
    """ Run the command and directly output to the console. """
    process = subprocess.Popen(command, stdout=subprocess.PIPE)
    while True:
        output = process.stdout.readline()
        if output == b'' and process.poll() is not None:
            break
        if output:
            print(output.decode(locale.getdefaultlocale()[1]), end='')
    return process.poll()


if __name__ == '__main__':
    run_command(['py', '-3.10', '-m', 'pip', 'install',
                '-r', 'build_requirements.txt'])
    VERSION_DATE, VERSION_HASH, VERSION_URL = generate_version_information()
    name = f'electroLABBOT ({VERSION_DATE.replace(":", "-")})'

    run_command(['py', '-3.10', '-m', 'pip', 'install',
                '-r', './py_src/requirements.txt'])
    run_command(['py', '-3.10', './py_src/ui_to_py_converter.py',
                './py_src/vue_principale.ui', './py_src/vue_principale.py'])
    run_command(['py', '-3.10', '-m', 'PyInstaller', '-F', '--workpath',
                 './', '--distpath', './', '--specpath', './py_src/build',
                 '--clean', '--add-data', '../package_data;package_data',
                 '-n', name, '--windowed', '--icon=../package_data/icon.ico',
                 './py_src/gui.pyw'])
    shutil.rmtree(name)
