cmake_minimum_required(VERSION 3.22)
project(La2a VERSION 0.1.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "" FORCE)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)

include(FetchContent)
FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.6)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(La2a
  COMPANY_NAME "Shoup Audio"
  BUNDLE_ID "audio.shoup.la2a"
  PLUGIN_MANUFACTURER_CODE Shup
  PLUGIN_CODE L2Aa
  FORMATS AU VST3 Standalone
  PRODUCT_NAME "LA2A"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(La2a)

target_sources(La2a PRIVATE
  Source/PluginProcessor.cpp
  Source/PluginProcessor.h
  Source/PluginEditor.cpp
  Source/PluginEditor.h
  Source/OptoTubeStage.h)

target_compile_definitions(La2a PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(La2a PRIVATE
  juce::juce_audio_utils
  juce::juce_dsp
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags)
