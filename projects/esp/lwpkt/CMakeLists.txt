idf_component_register(
  SRC_DIRS "../../../libraries/lwpkt/lwpkt/src/lwpkt" "../../../libraries/lwpkt/libs/lwrb/src/lwrb"
  INCLUDE_DIRS "../../../libraries/lwpkt/lwpkt/src/include" "../../../libraries/lwpkt/libs/lwrb/src/include")

target_compile_definitions(${COMPONENT_LIB} PRIVATE LWPKT_IGNORE_USER_OPTS)
