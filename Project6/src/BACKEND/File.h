// File.h
#ifndef FILE_H
#define FILE_H

#include <string>

class File {
	private:
		int fd;
		std::string type;
		std::string object;
	public:
		File() {
			this->fd = -1;
			this->type = "";
			this->object = "";
		}
		File(std::string path); // TODO: create the file object coresponding to the path.
		int get_fd() {
			return this->fd;
		}
		void set_fd(int fd) {
			this->fd = fd;
		}
		std::string get_type() {
			return this->type;
		}
		void set_type(std::string type) {
			this->type = type;
		}
		std::string get_object() {
			return this->object;
		}
		void set_object(std::string object) {
			this->object = object;
		}
		// What do you mean by object? Like an open file*?
};

#endif
