#include "stdafx.h"
#include <exception>
#include "input_msu.h"

#define MSU_MAGIC "MSU1"

// larger values cause seeking distortion [ms]
static const uint32_t BUFFER_DURATION = 100;

input_msu::input_msu()
{
	m_PcmFilePos = 0;
	m_PcmFileLoop = 0;
	m_PcmFileLength = 0;
}

input_msu::~input_msu()
{
	
}

/**
* Open file
*
* @param p_filehint
* @param p_path
* @param p_reason
* @param p_abort
*
*/
void input_msu::open(service_ptr_t<file> p_filehint, const char *p_path, t_input_open_reason p_reason, abort_callback &p_abort)
{
	m_File = p_filehint;
	m_PcmPath.set_string(p_path);
	input_open_file_helper(m_File, p_path, p_reason, p_abort);

	// read spc file
	m_PcmFileLength = static_cast<uint32_t>(m_File->get_size(p_abort));
	m_PcmFile.set_size(m_PcmFileLength);
	m_File->read(m_PcmFile.get_ptr(), m_PcmFileLength, p_abort);

	// check header
	if (::memcmp(m_PcmFile.get_ptr(), MSU_MAGIC, 4)) {
		throw exception_io_unsupported_format();
	}
}

/**
* Get track info
*
* @param p_info
* @param p_abort
*
*/
void input_msu::get_info(file_info &p_info, abort_callback &p_abort)
{
	p_info.set_length((m_PcmFileLength - 8.0) / (44100.0 * 4.0));
	
	p_info.info_set_int("samplerate", 44100);
	p_info.info_set_int("channels", 2);
	p_info.info_set_int("bitspersample", 16);

	std::string p = m_PcmPath.toString();
	std::string t = p.substr(p.find_last_of("-") + 1, p.find_last_of(".") - p.find_last_of("-") - 1);
	p_info.meta_add("tracknumber", t.c_str());
}

/**
* Initialize for decode
*
* @param p_flags
* @param p_abort
*
*/
void input_msu::decode_initialize(unsigned int p_flags, abort_callback &p_abort)
{
	m_PcmFilePos = 8;
	m_PcmFileLoop = (*((uint32_t *)(m_PcmFile.get_ptr() + 4)) * 4) + 8;
}

/**
* Decode
*
* @param p_chunk
* @param p_abort
* @return true / false
*/
bool input_msu::decode_run(audio_chunk &p_chunk, abort_callback &p_abort)
{
	uint32_t wanted_ms = BUFFER_DURATION;
	int16_t buf[44100 * BUFFER_DURATION * 4 / 1000];

	if (m_PcmFilePos >= m_PcmFileLength)
	{
		if (m_PcmFileLoop < m_PcmFileLength)
		{
			m_PcmFilePos = m_PcmFileLoop;
		}
		else
		{
			return false;
		}
	}

	uint32_t buffer_size = m_PcmFilePos + sizeof(buf) < m_PcmFileLength ? sizeof(buf) : m_PcmFileLength - m_PcmFilePos;

	//memcpy(buf, m_PcmFile.get_ptr() + m_PcmFilePos, buffer_size);

	if (buffer_size < sizeof(buf))
	{
		buffer_size -= 0;
	}

	p_chunk.set_data_int16(reinterpret_cast<int16_t *>(m_PcmFile.get_ptr() + m_PcmFilePos), buffer_size / 4, 2, 44100, audio_chunk::channel_config_stereo);

	m_PcmFilePos += buffer_size;

	return true;
}

/**
* Seek
*
* @param p_seconds
* @param p_abort
*/
void input_msu::decode_seek(double p_seconds, abort_callback &p_abort)
{
	// compare desired seek pos with current pos
	uint32_t NewPos = static_cast<uint32_t>(p_seconds * 4 * 44100) & ~0x0007;

	m_PcmFilePos = NewPos + 8 <= m_PcmFileLength ? NewPos + 8 : m_PcmFileLength;
}

static input_singletrack_factory_t<input_msu> g_input_msu_factory;

// version info
DECLARE_COMPONENT_VERSION("MSU-1 PCM Player",
"0.1",
"MSU-1 audio playback plug-in.\n(c) 2017, qwertymodo")
DECLARE_FILE_TYPE("MSU-1 audio files", "*.pcm");

// This will prevent users from renaming your component around (important for proper troubleshooter behaviors) or loading multiple instances of it.
VALIDATE_COMPONENT_FILENAME("foo_input_msu.dll");