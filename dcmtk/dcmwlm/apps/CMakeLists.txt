# declare additional include directories
INCLUDE_DIRECTORIES(${dcmtls_SOURCE_DIR}/include)

# declare directories containing used libraries
LINK_DIRECTORIES(${dcmwlm_BINARY_DIR} ${ofstd_BINARY_DIR} ${dcmdata_BINARY_DIR} ${dcmtls_BINARY_DIR} ${dcmnet_BINARY_DIR} ${ZLIB_LIBDIR})

# declare executables
ADD_EXECUTABLE(wlmscpfs wlmscpfs wlcefs)

# declare installation files
INSTALL_TARGETS(${INSTALL_BINDIR} wlmscpfs)

# make sure executables are linked to the corresponding libraries
TARGET_LINK_LIBRARIES(wlmscpfs netapi32 wsock32 ofstd dcmdata dcmtls dcmnet dcmwlm ${ZLIB_LIBS})
