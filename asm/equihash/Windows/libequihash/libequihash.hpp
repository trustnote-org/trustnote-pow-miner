struct cpu_xenoncat
{
	std::string getdevinfo() { return ""; }

	static void start( cpu_xenoncat & device_context );

	static void stop( cpu_xenoncat & device_context );

	static void solve( const char * tequihash_header,
		unsigned int tequihash_header_len,
		const char * nonce,
		unsigned int nonce_len,
		cpu_xenoncat & device_context );

	std::string getname() 
	{
		if (use_opt) return "CPU-XENONCAT-AVX2";
		else return "CPU-XENONCAT-AVX";
	}

	void *memory_alloc, *memory;
	int use_opt;
};