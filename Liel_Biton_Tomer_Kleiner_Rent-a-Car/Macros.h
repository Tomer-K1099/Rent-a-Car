#pragma once

#define CLOSE_RETURN_NULL(fp) {fclose(fp);return 0;}
#define CLOSE_FILE_IF_PTR_NULL(ptr,fp) {if(!ptr) CLOSE_RETURN_NULL(fp)}
#define CLOSE_FILE_AND_FREE(ptr,fp) {free(ptr); CLOSE_RETURN_NULL(fp)}
#define ERROR_ALOC_RETURN_NULL(ptr){if(!ptr) {return 0;}}