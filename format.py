import subprocess

binary = 'clang-format'
file_extensions = [
    '*.cpp',
    '*.h',
]

def main():
    files_command = ['git', 'ls-files'] + file_extensions
    files = subprocess.check_output(files_command, shell=True).splitlines()

    format_command = [binary, '-i'] + files
    subprocess.check_output(format_command, shell=True)

main()
