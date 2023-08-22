include "dependencies.lua"

workspace "ReviveNES"
	architecture "x86_64"
	startproject "ReviveNES"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "ReviveNES/vendor/Glad"
	include "ReviveNES/vendor/glfw"
	include "ReviveNES/vendor/imgui"
	include "ReviveNES/vendor/stb"
group ""

include "ReviveNES"