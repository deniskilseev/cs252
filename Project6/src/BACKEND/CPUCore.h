// CPUCore.h
#ifndef CPUCore_H
#define CPUCore_H

class CPUCore {
	private:
		double load;
		unsigned long uptime;
		unsigned long downtime;
	public:
		CPUCore() {
			this->load = 0;
			this->uptime = 0;
			this->downtime = 0;
		}

		double get_load() {
			return this->load;
		}

		void set_load(double load) {
			this->load = load;
		}

		unsigned long get_uptime() {
			return this->uptime;
		}

		void set_uptime(unsigned long uptime) {
			this->uptime = uptime;
		}

		unsigned long get_downtime() {
			return this->downtime;
		}

		void set_downtime(unsigned long downtime) {
			this->downtime = downtime;
		}

		void print();
};
#endif
