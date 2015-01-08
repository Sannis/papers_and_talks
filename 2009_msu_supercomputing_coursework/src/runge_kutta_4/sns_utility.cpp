#include "sns_utility.h"

/**
* @todo	Write doccomment
*/
namespace sns
{
	/**
	* @todo	Write doccomment
	*/
	namespace utility
	{
		char* get_results_path(int filename_max_length, char* results_root_path, char* subfolder)
		{
			char* results_path = new char[filename_max_length];
			
			if( !results_path) {
				return NULL;
			}
			
			sprintf(results_path, "rm -rf %s/%s/*", results_root_path, subfolder);
			system(results_path);
			
			sprintf(results_path, "mkdir -p %s/%s", results_root_path, subfolder);
			system(results_path);
			
			sprintf(results_path, "%s/%s", results_root_path, subfolder);
			
			return results_path;
		}
		
		void write_log(char* filename, char* log)
		{
			FILE *fp = fopen(filename, "a");
			
			if ( !fp ) {
				printf("ERROR: Can't write program execution log. Error code: 1.\n");
				return;
			}
			
			if ( fputs(log, fp) == EOF ) {
				printf("ERROR: Can't write program execution log. Error code: 2.\n");
			}
			
			fclose(fp);
		}
	}
}

