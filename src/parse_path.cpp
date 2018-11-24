#include "parse_path.hpp"

void parsePath(string full_path, string& filename, string& path) {
	filename.clear();
	for(int i = 0; i < (int)full_path.size(); i++) {
		char c = full_path[i];
		if (c == '\' || c == '//') {
			path += filename;
			path += c;
			filename.clear();
		} else {
			filename += c;
		}
	}
}