package=cmake
$(package)_version=3.14.7
$(package)_download_path=https://cmake.org/files/v3.14/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=9221993e0af3e6d10124d840ff24f5b2f3b884416fca04d3312cb0388dec1385

define $(package)_set_vars
  $(package)_build_opts=CC="$($(package)_cc)"
  $(package)_build_env+=CFLAGS="$($(package)_cflags) $($(package)_cppflags)" AR="$($(package)_ar)"
endef

define $(package)_config_cmds
  export CC="$($(package)_cc)" && \
  export CXX="$($(package)_cxx)" && \
  export CFLAGS="$($(package)_cflags) $($(package)_cppflags)" && \
  export CXXFLAGS="$($(package)_cxxflags) $($(package)_cppflags)" && \
  export LDFLAGS="$($(package)_ldflags)" && \
  ./bootstrap --prefix=$(host_prefix)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef