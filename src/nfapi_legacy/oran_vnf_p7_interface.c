#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

nfapi_vnf_p7_config_t* nfapi_vnf_p7_config_create()
{
	vnf_p7_t* _this = (vnf_p7_t*)calloc(1, sizeof(vnf_p7_t));

	if(_this == 0)
		return 0;

	// todo : initialize
	_this->_public.segment_size = 1400;
	_this->_public.max_num_segments = 8;
	_this->_public.checksum_enabled = 1;

	_this->_public.malloc = &malloc;
	_this->_public.free = &free;

	_this->_public.codec_config.allocate = &malloc;
	_this->_public.codec_config.deallocate = &free;


	return (nfapi_vnf_p7_config_t*)_this;
}