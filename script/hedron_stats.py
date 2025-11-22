import os

def count_lines_of_code(directory, file_extensions):
	"""
	Recursively count the lines of code in all files within a directory tree
	that match the given file extensions.

	:param directory: The root directory to start the search.
	:param file_extensions: A tuple of file extensions to include, e.g., (".py", ".cpp", ".h").
	:return: Total number of lines of code.
	"""
	total_lines = 0

	for root, _, files in os.walk(directory):
		for file in files:
			if file.endswith(file_extensions):
				file_path = os.path.join(root, file)
				try:
					with open(file_path, 'r', encoding='utf-8') as f:
						lines = f.readlines()
						# Filter out empty lines and comment lines (optional)
						code_lines = [line for line in lines if line.strip() and not line.strip().startswith("#")]
						total_lines += len(code_lines)
				except Exception as e:
					print(f"Error reading file {file_path}: {e}")

	return total_lines

if __name__ == "__main__":
	base_path = os.getcwd()
	target_directory = os.path.join(base_path, "module")

	# File extensions to include
	extensions = (".py", ".cpp", ".h", ".java", ".cs")

	# Calculate total lines of code
	total_loc = count_lines_of_code(target_directory, extensions)

	print(f"Total lines of code in '{target_directory}' (extensions: {extensions}): {total_loc}")
