idf_component_register(
  SRC_DIRS "../../../src/frontend" "../../../src/frontend/ui" "../../../src/frontend/ui/menu" "../../../src/frontend/connection" "../../../src/shared"
  INCLUDE_DIRS "../../../src" "../../../libraries/dc2" "../../../libraries/lwpkt/lwpkt/src/include" "../../../libraries/lwpkt/libs/lwrb/src/include"
  REQUIRES esp_driver_uart
  REQUIRES lwpkt
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LWPKT_IGNORE_USER_OPTS)
