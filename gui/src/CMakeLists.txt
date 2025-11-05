list(APPEND WEBHACKER_INCLUDES
)

list(APPEND WEBHACKER_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# location for *.qm file, overrides build folder
set(TS_OUTPUT_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../resources)

# run lrelease to generate translation files
#COPPERSPICE_RESOURCES(
#   ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks_en.ts
#   ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks_de.ts
#   ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks_fr.ts
#)

# run rcc to generate qrc output
#COPPERSPICE_RESOURCES(
  # ${CMAKE_CURRENT_SOURCE_DIR}/../WebHacker.qrc
#)

# run uic to generate source
#COPPERSPICE_RESOURCES(

#)

add_executable(WebHacker ${WEBHACKER_SOURCES})
target_include_directories(WebHacker
   PRIVATE
   ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(WebHacker
   CopperSpice::CsCore
   CopperSpice::CsGui
   CopperSpice::CsNetwork
   CopperSpice::CsSql
   CopperSpice::CsSvg
   CopperSpice::CsXml
   CopperSpice::CsXmlPatterns
   WhCore
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
   set_target_properties(WebHacker PROPERTIES OUTPUT_NAME WebHacker)

   target_compile_definitions(WebHacker
      PUBLIC
      GL_SILENCE_DEPRECATION
   )

   target_link_libraries(WebHacker
      "-framework OpenGL"
   )

elseif(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD|DragonFly)")
   set_target_properties(WebHacker PROPERTIES OUTPUT_NAME WebHacker)

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
   target_link_libraries(WebHacker
      netapi32
      mpr
   )

   if (MSVC)
      # disables the console window
      target_link_options(WebHacker
         PRIVATE
         /subsystem:windows
         /entry:mainCRTStartup
      )

   else()
      # disables the console window
      target_link_libraries(WebHacker
         -mwindows
      )

   endif()

endif()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
   install(TARGETS WebHacker DESTINATION ../MacOS)
else()
   install(TARGETS WebHacker DESTINATION .)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/sampleMenu.xml  DESTINATION ./resources)

# installs the CopperSpice libraries
cs_copy_library(CsCore)
cs_copy_library(CsGui)
cs_copy_library(CsNetwork)
cs_copy_library(CsSql)
cs_copy_library(CsSvg)
cs_copy_library(CsXml)
cs_copy_library(CsXmlPatterns)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

   set(CPACK_DMG_VOLUME_NAME WebHacker)
   set(CPACK_DMG_FORMAT UDBZ)

   # installs the platform Gui plugin
   cs_copy_plugins(CsGui ../plugins)

 #  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks.icns DESTINATION ./resources)

elseif(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD|DragonFly)")

   # installs the platform Gui plugin
   cs_copy_plugins(CsGui)
   cs_copy_plugins(CsOpenGL)

 #  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks.png DESTINATION ./resources)

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")

   # installs the platform Gui plugin
   cs_copy_plugins(CsGui)


 #  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ks.ico DESTINATION ./resources)

endif()

