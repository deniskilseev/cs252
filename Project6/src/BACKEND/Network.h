// Network.h
#ifndef NETWORK_H
#define NETWORK_H
class Network {
	private:
		unsigned long uploadSpeed; // MiB/Second
		unsigned long downloadSpeed;
	public:
		Network() {
			this->uploadSpeed = 0;
			this->downloadSpeed = 0;
		}

		unsigned long get_upload_speed() {
			return this->uploadSpeed;
		}

		void set_upload_speed(unsigned long upload) {
			this->uploadSpeed = upload;
		}

		unsigned long get_download_speed() {
			return this->downloadSpeed;
		}

		void set_download_speed(unsigned long download) {
			this->downloadSpeed = download;
		}

		void print(); // DEBUG ONLY

};
#endif
