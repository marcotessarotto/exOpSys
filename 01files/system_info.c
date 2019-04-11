#include <stdio.h>
#include <sys/sysinfo.h>

#define MB (1024*1024)

void print_system_info() {

	static struct sysinfo prev_info; // inizializzata a zero per default, perchè è 'static'

	struct sysinfo info;

	// man 2 sysinfo
	sysinfo(&info);


	if (prev_info.freeram != 0) {

		double delta_freeram = ((long)info.freeram - (long)prev_info.freeram) * 100. / info.freeram;
		double delta_sharedram = ((long)info.sharedram - (long)prev_info.sharedram) * 100. / info.sharedram;
		double delta_bufferram = ((long)info.bufferram - (long)prev_info.bufferram) * 100. / info.bufferram;
		double delta_freeswap = ((long)info.freeswap - (long)prev_info.freeswap) * 100. / info.freeswap;


		printf("freeram=%lu MB(%.2lf %%) totalram=%lu MB sharedram=%lu MB bufferram=%lu MB totalswap=%lu MB freeswap=%lu MB\n",
						info.freeram / MB, delta_freeram,
						info.totalram / MB, info.sharedram / MB, info.bufferram / MB, info.totalswap / MB, info.freeswap / MB);

	} else {

		printf("freeram=%lu MB totalram=%lu MB sharedram=%lu MB bufferram=%lu MB totalswap=%lu MB freeswap=%lu MB\n",
						info.freeram / MB,
						info.totalram / MB, info.sharedram / MB, info.bufferram / MB, info.totalswap / MB, info.freeswap / MB);
	}

	prev_info = info;

}


unsigned long get_freeram() {

	struct sysinfo info;

	// man 2 sysinfo
	sysinfo(&info);

	return info.freeram;
}
