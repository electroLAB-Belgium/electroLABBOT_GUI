""" This module contains all the useful functions of this project. """
import os
import re
import sys
from functools import reduce

import esptool
from PyQt6.QtCore import QObject, pyqtSignal

# Files and directories related functions


def list_files(path: str):
    """List all the files in a folder and subfolders.

    Args:
        path: the path to use as parent directory.
    Returns:
        A list of files.
    """
    files_list = set()

    for folder, _, files in os.walk(path):
        for file_ in files:
            files_list.add(os.path.join(folder, file_))

    return list(files_list)


def ensure_path(path: str):
    """ Ensure the existence of a specified path.

    Args:
        path: path to create if non existing.
    """
    if not os.path.isdir(path):
        os.makedirs(path)


def find_files(basename_prefix: str, sources: dict):
    """Find the path to the file if it exists.

    Args:
        basename_prefix: the basename prefix of the recording.
        sources: the sources to use.
    Returns:
        An empty list or a list of string which contains the path to the files.
    """
    for files in sources.values():
        matching_files = [
            file_ for file_ in files
            if os.path.basename(file_).startswith(basename_prefix + '_')
        ]

        if matching_files:
            return matching_files
    return []


# Args parsing related functions
def display_arguments(args, message: str = ''):
    """ Display the parsed arguments. """
    if message is not None and message == '':
        print('The following arguments have been parsed:')
    elif message is not None and message != '':
        print(message)

    for key, value in vars(args).items():
        print('{0}: {1}'.format(key, value))


def handle_yes_no(args):
    """ Handle the arguments 'yes' and 'no'. """
    if not args.no:
        print()
        if not args.yes:
            try:
                while True:
                    conti = input('Do you want to run the program (yes/no)? ')
                    if conti.lower() in ('n', 'no'):
                        sys.exit()

                    elif conti.lower() in ('y', 'yes'):
                        break

            except KeyboardInterrupt:
                print(
                    '\nThe user requested the end of the program'
                    ' (KeyboardInterrupt).',
                )

                sys.exit()
    else:
        sys.exit()


# String processing related function
def split_keep_sep(string, separator):
    """Split a string according to a separator.

    Args:
        string: the string to split.
        separator: the separator to use and to keep.

    Returns:
        A list with the spliced elements.
    """
    return reduce(
        lambda acc, elem: acc[:-1] + [acc[-1] + elem] if elem == separator
        else acc + [elem], re.split('(%s)' % re.escape(separator), string), [],
    )


# PyInstaller related function
def resource_path(relative_path: str):
    """Get absolute path to resource, works for dev and for PyInstaller.

    Args:
        relative_path: the path to resolve.

    Returns:
        The absolute path to the ressource.
    """
    base_path = getattr(
        sys,
        '_MEIPASS',
        os.path.dirname(os.path.abspath(__file__)),
    )

    return os.path.join(base_path, relative_path)


def flash(command: list):
    """Flash the ESP32.

    This function is wrapping the esptool.main() function.

    Args:
        command: a list of string whom constitute the command.

    Returns:
        True if the command succeed, False otherwise.
    """
    try:
        esptool.main(command)
        return True

    except Exception as error:
        print(f'\nError:\n{error}\n')
        # import traceback
        # traceback.print_exc()
        return False


# https://stackoverflow.com/a/11764381/10949679
class XStream(QObject):
    """Capture the stdout stream."""
    _stdout = None
    _stderr = None

    message_written = pyqtSignal(str)

    @staticmethod
    def isatty():
        """The stream is interactive."""
        return True

    @staticmethod
    def flush():
        """Flush stream, here, has no effect."""
        return

    @staticmethod
    def fileno():
        """Return fileline."""
        return -1

    def write(self, text: str):
        """Write in the stream.

        Args:
            self: self.
            text: string to write in the stream.
        """
        if not self.signalsBlocked():
            self.message_written.emit(text)

    @staticmethod
    def stdout():
        """Capture sys.stdout in this stream.

        Returns:
            The new XStream._stdout attribute.
        """
        if not XStream._stdout:
            XStream._stdout = XStream()
            sys.stdout = XStream._stdout
        return XStream._stdout

    @staticmethod
    def stderr():
        """Capture sys.stderr in this stream.

        Returns:
            The new XStream._stderr attribute.
        """
        if not XStream._stderr:
            XStream._stderr = XStream()
            sys.stderr = XStream._stderr
        return XStream._stderr


if __name__ == '__main__':
    print('utils.py is a module, not a script')
