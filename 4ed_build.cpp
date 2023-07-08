/*
 * Mr. 4th Dimention - Allen Webster
 *
 * ??.??.????
 *
 * 4coder development build rule.
 *
 */

// TOP

#define FM_PRINT_COMMANDS

#include "4coder_base_types.h"
#include "4coder_version.h"
#include "4coder_base_types.cpp"
#include "4coder_malloc_allocator.cpp"
#define FTECH_FILE_MOVING_IMPLEMENTATION
#include "4coder_file_moving.h"

// NOTE(allen): Build flags
enum{
    OPTS = 0x1,
    LIBS = 0x2,
    ICON = 0x4,
    SHARED_CODE = 0x8,
    DEBUG_INFO = 0x10,
    OPTIMIZATION = 0x20,
    SUPER = 0x40,
    INTERNAL = 0x80,
    SHIP = 0x100,
};

struct Project
{
	struct Layout
	{
		char* project_root_path;
		char* core_layer_path;
		char* custom_layer_path;
		char* build_path;
		char* ship_files_path;
		char* ship_themes_path;
		char* build_themes_path;
		char* site_path;
		char* distributions_path;
		char* non_source_path;
		char* dist_files_path;
		char* foreign_path;

		char* package_path;
		char* zip_path;
		char* zip_name;
	};

	struct Compilation
	{
		char* app_target;
		char* app_target_out;
		char* platform_layer;
		char* platform_layer_out;
		char* custom_layer;
		char* custom_layer_out;

		char* compiler_options;
		char* debug_options;
		char* optimization_options;
		char* arch_options;

		char* defines;
		char* exports;
		char* library;
		char* icon;

		char* so_includes;
		char* bin_includes;

		u32 flags;
		char* os;
		char* compiler;
	};

	Layout layout;
	Compilation compilation;
};

#define BoldWhite(formatted_message, ...)\
do\
{\
printf("\033[1m\x1B[97m");\
printf(formatted_message, __VA_ARGS__);\
printf("\033[0m");\
fflush(stdout);\
}while(0);\

#if OS_WINDOWS
char* platform_layer_main_file = "platform_win32" SLASH "win32_4ed.cpp";
#elif OS_LINUX
char* platform_layer_main_file = "platform_linux" SLASH "linux_4ed.cpp";
#elif OS_MAC
char* platform_layer_main_file = "platform_mac"   SLASH "mac_4ed.mm";
#endif


#if COMPILER_CL

#define compiler_flags "-W4 -wd4310 -wd4100 -wd4201 -wd4505 -wd4996 -wd4127 -wd4510 -wd4512 -wd4610 -wd4390 -wd4611 -wd4189 -WX -GR- -EHa- -nologo -FC -link -INCREMENTAL:NO -RELEASE -PDBALTPATH:%%_PDB%%"
#define debug_flags " -Zi -DDO_CRAZY_EXPENSIVE_ASSERTS"
#define optimization_flags " -O2"
#define arch_flags " -MACHINE:X64 -DFTECH_64_BIT"
#define libraries "user32.lib winmm.lib gdi32.lib opengl32.lib comdlg32.lib userenv.lib .\\non-source\\foreign\\x64\\freetype.lib .\\non-source\\foreign\\x86\\freetype.lib -NODEFAULTLIB:library"
#define exports " -OPT:REF -EXPORT:"
#define icon_flags ".\\non-source\\res\\icon.res"
#define SHARED_FLAG " -LD"
#define REMOVE_PROGRAM "del "
#define PREPROCESS_FLAG " -E"
#define OUT_FLAG " -o "
#define INCLUDE_FLAG " -I"
#define DEFINE_FLAG " -D"

//internal void
//build(Arena *arena, u32 flags, u32 arch, char *code_path, char **code_files, char *out_path, char *out_file, char **defines, char **exports, char **inc_folders){
//    Temp_Dir temp = fm_pushdir(out_path);

//    Build_Line line;
//    fm_init_build_line(&line);

//    if (arch == Arch_X86){
//        fm_add_to_line(line, "%s\\custom\\bin\\setup_cl_x86.bat &", code_path);
//    }

//    fm_add_to_line(line, "cl");

#elif COMPILER_GCC

#if OS_LINUX

#define compiler_flags "-Wno-write-strings -D_GNU_SOURCE -fPIC -fno-threadsafe-statics -pthread -Wno-unused-result -std=c++11"
#define debug_flags "-g3 -ggdb3 -O0 -fno-eliminate-unused-debug-types -fvar-tracking -fno-eliminate-unused-debug-symbols"
#define optimization_flags " -O3"
#define arch_flags " -m64 -DFTECH_64_BIT"
#define libraries "-lX11 -lpthread -lm -lrt -lGL -ldl -lXfixes -lfreetype -lfontconfig"
#define icon_flags " "
#define SHARED_FLAG " -shared -fPIC"
#define REMOVE_PROGRAM "rm "
#define PREPROCESS_FLAG " -E"
#define OUT_FLAG " -o "
#define INCLUDE_FLAG " -I"
#define DEFINE_FLAG " -D"

#else
# error gcc options not set for this platform
#endif

#elif COMPILER_CLANG

#if OS_MAC
# define libraries "-framework Cocoa -framework QuartzCore -framework CoreServices -framework OpenGL -framework IOKit -framework Metal -framework MetalKit ./non-source/foreign/x86/libfreetype-mac.a ./non-source/foreign/x64/libfreetype-mac.a"
#elif OS_LINUX
# define libraries "-lX11 -lpthread -lm -lrt -lGL -ldl -lXfixes -lfreetype -lfontconfig"
#else
# error clang options not set for this platform
#endif

#define compiler_flags "-Wno-write-strings -Wno-null-dereference -Wno-switch -Wno-missing-declarations -Wno-format-extra-args -D_GNU_SOURCE -fPIC -fno-threadsafe-statics -pthread -Wno-unused-result -std=c++11"
#define debug_flags "-g3 -ggdb3 -O0 -fno-eliminate-unused-debug-types "
#define optimization_flags " -O3"
#define arch_flags " -m64 -DFTECH_64_BIT"
#define icon_flags " "
#define SHARED_FLAG " -shared -fPIC"
#define REMOVE_PROGRAM "rm "
#define PREPROCESS_FLAG " -E"
#define OUT_FLAG " -o "
#define INCLUDE_FLAG " -I"
#define DEFINE_FLAG " -D"


#elif COMPILER_CIRCLE

# if OS_LINUX

#  define compiler_flags "-Wno-writable-strings -Wno-deprecated-declarations -Wno-comment -Wno-switch -Wno-null-dereference  -Wno-tautological-compare  -std=c++11"
#  define debug_flags "-g -O0  -fvar-tracking -fno-eliminate-unused-debug-symbols"
#  define optimization_flags " -O3"
#  define arch_flags " -m64 -DFTECH_64_BIT"
#  define libraries "-lX11 -lpthread -lm -lrt -lGL -ldl -lXfixes -lfreetype -lfontconfig"
#  define icon_flags " "
#  define SHARED_FLAG " -shared -fPIC"
#  define REMOVE_PROGRAM "rm "
#  define PREPROCESS_FLAG " -E"
#  define OUT_FLAG " -o "
#  define INCLUDE_FLAG " -I"
#  define DEFINE_FLAG " -D"

#else
# error build function not defined for this compiler
#endif

#endif

internal void
buildsuper(Arena *arena, const Project* project)
{
	const Project::Layout* layout =  &project->layout ;
	const Project::Compilation* compilation =  &project->compilation;

	BoldWhite("\n*-*-* Generate the preproc file for the metadata generator *-*-*\n", "");
	char* include_home_folder = fm_str(arena, INCLUDE_FLAG, layout->custom_layer_path);
	char* source = compilation->custom_layer;
	char* meta_macros = DEFINE_FLAG "META_PASS";
	char* arch_flag = compilation->arch_options;
	char* opts = compilation->compiler_options;
	char* debug = compilation->debug_options;
	char* preproc_file = fm_str(arena, layout->custom_layer_path, SLASH, "4coder_command_metadata.i ");
	systemf("%s %s %s %s %s %s %s %s %s %s",
			compilation->compiler,
			include_home_folder,
			meta_macros,
			arch_flag,
			opts,
			debug,
			source,
			PREPROCESS_FLAG,
			OUT_FLAG,
			preproc_file);

	BoldWhite("\n*-*-* Build the metadata generator *-*-*\n", "");
	char* metadata_generator_cpp = fm_str(arena, layout->custom_layer_path, SLASH, "4coder_metadata_generator.cpp");
	char* metadata_generator     = fm_str(arena, layout->custom_layer_path, SLASH, "metadata_generator");
	systemf("%s %s %s %s %s %s %s",
			compilation->compiler,
			include_home_folder,
			opts,
			debug,
			metadata_generator_cpp,
			OUT_FLAG,
			metadata_generator);

	BoldWhite("\n*-*-* Generate meta data *-*-*\n", "");
	char* home_folder = layout->custom_layer_path;
	systemf("%s -R %s %s",
			metadata_generator,
			home_folder,
			preproc_file);

	BoldWhite("\n*-*-* Build the custom layer: %s *-*-*\n", source);
	char* shared_library = compilation->custom_layer_out;
	systemf("%s %s %s %s %s %s %s %s %s",
			compilation->compiler,
			include_home_folder,
			arch_flag,
			opts,
			debug,
			source,
			SHARED_FLAG,
			OUT_FLAG,
			shared_library);

	BoldWhite("\n*-*-* Clear temporary files *-*-*\n", "");
	systemf("%s %s", REMOVE_PROGRAM, metadata_generator);
	systemf("%s %s", REMOVE_PROGRAM, preproc_file);
}

internal void
build(Arena *arena, u32 flags, const Project::Layout* layout, const Project::Compilation* compilation)
{
    Build_Line line;
    fm_init_build_line(&line);

	fm_add_to_line(line, " %s", compilation->arch_options);
	fm_add_to_line(line, " %s", compilation->compiler_options);
	fm_add_to_line(line, " %s", compilation->defines);

    fm_add_to_line(line, "%s%s", INCLUDE_FLAG, layout->core_layer_path);
	fm_add_to_line(line, "%s%s", INCLUDE_FLAG, layout->custom_layer_path);

    if (flags & DEBUG_INFO) { fm_add_to_line(line, "", compilation->debug_options); }
	if (flags & OPTIMIZATION) { fm_add_to_line(line, "", compilation->optimization_options); }

	if(flags & SHARED_CODE)
	{
		fm_add_to_line(line, SHARED_FLAG);
		fm_add_to_line(line, " %s", compilation->so_includes);
		fm_add_to_line(line, " %s", compilation->app_target);
		fm_add_to_line(line, OUT_FLAG);
		fm_add_to_line(line, " %s", compilation->app_target_out);
	}
	else
	{
		fm_add_to_line(line, " %s", compilation->bin_includes);
		fm_add_to_line(line, " %s", compilation->library);
		fm_add_to_line(line, " %s", compilation->icon);
		fm_add_to_line(line, " %s", compilation->platform_layer);
		fm_add_to_line(line, OUT_FLAG);
		fm_add_to_line(line, " %s", compilation->platform_layer_out);
	}

    fm_finish_build_line(&line);

    systemf("%s %s", compilation->compiler, line.build_options);
}

internal void
build_main(Arena *arena, const Project* project, b32 update_local_theme)
{
	u32 flags = project->compilation.flags;

	// Build the 4ed_app - shared library
	BoldWhite("\n*-*-* Build the 4ed library (%s -> %s) *-*-*\n", project->compilation.custom_layer, project->compilation.custom_layer_out);
	build(arena, SHARED_CODE | flags, &project->layout, &project->compilation);

	// Build the 4ed binary
	BoldWhite("\n*-*-* Build the 4ed binary (%s -> %s) *-*-*\n", project->compilation.platform_layer, project->compilation.platform_layer_out);
	build(arena, LIBS | flags, &project->layout, &project->compilation);

    if (update_local_theme)
	{
        char *themes_folder = project->layout.build_themes_path;
        char *source_themes_folder = project->layout.ship_themes_path;

		BoldWhite("\n*-*-* Clear: %s  *-*-*\n", themes_folder);
		fm_clear_folder(themes_folder);

		BoldWhite("\n*-*-* Make if missing: %s  *-*-*\n", themes_folder);
		fm_make_folder_if_missing(arena, themes_folder);

		BoldWhite("\n*-*-* Copying all from: %s  to: %s *-*-*\n", source_themes_folder, themes_folder);
		fm_copy_all(source_themes_folder, themes_folder);
    }
}

//function void
//package_for_arch(Arena *arena, u32 arch, char *cdir, char *build_dir, char *pack_dir, i32 tier, char *tier_name,  char *current_dist_tier, u32 flags, char** dist_files, i32 dist_file_count){
//    char *arch_name  = "x64";
//    char *parent_dir = fm_str(arena, current_dist_tier, "_", arch_name);
//    char *dir        = fm_str(arena, parent_dir, SLASH "4coder");
//    char *zip_dir    = fm_str(arena, pack_dir, SLASH, tier_name, "_", arch_name);

//    printf("\nBUILD: %s_%s\n", tier_name, arch_name);
//    printf(" parent_dir = %s;\n", parent_dir);
//    printf(" dir = %s;\n", dir);
//    printf(" zip_dir = %s;\n", zip_dir);
//    fflush(stdout);

//	// UPDATE THIS CALL TO USE THE NEW PROJECT_LAYOUT STRUCT
//    //buildsuper(arena, cdir /*this should be project root*/, cdir /*this should be custom layer path*/, fm_str(arena, default_custom_target), arch);
//    //build_main(arena, cdir /*this should be project root*/, cdir /*this should be custom layer path*/, false, flags, arch);

//    fm_clear_folder(parent_dir);
//    fm_make_folder_if_missing(arena, parent_dir);

//    fm_make_folder_if_missing(arena, dir);
//    fm_copy_file(fm_str(arena, build_dir, "/4ed" EXE), fm_str(arena, dir, "/4ed" EXE));
//    fm_copy_file(fm_str(arena, build_dir, "/4ed_app" DLL), fm_str(arena, dir, "/4ed_app" DLL));
//    fm_copy_file(fm_str(arena, build_dir, "/custom_4coder" DLL), fm_str(arena, dir, "/custom_4coder" DLL));

//    for (i32 j = 0; j < dist_file_count; j += 1){
//        fm_copy_all(dist_files[j], dir);
//    }

//	char *custom_src_dir = fm_str(arena, cdir, SLASH, "custom");
//	char *custom_dst_dir = fm_str(arena, dir, SLASH, "custom");
//	fm_make_folder_if_missing(arena, custom_dst_dir);
//	fm_copy_all(custom_src_dir, custom_dst_dir);

//    char *dist_name = get_4coder_dist_name(arena, 0, tier_name, 0);
//    char *zip_name = fm_str(arena, zip_dir, SLASH, dist_name, ".zip");
//    fm_make_folder_if_missing(arena, zip_dir);
//    fm_zip(parent_dir, "4coder", zip_name);
//}

internal void
package(Arena *arena, Project* project){
	// NOTE(allen): meta
	Project::Layout* layout = &project->layout;
	Project::Compilation* compilation = &project->compilation;

	compilation->flags |= SHIP | OPTIMIZATION | SUPER;
	BoldWhite("\n*-*-* Clear: %s  *-*-*\n", layout->distributions_path);
	fm_clear_folder(layout->distributions_path);
	BoldWhite("\n*-*-* Make if missing: %s  *-*-*\n", layout->distributions_path);
	fm_make_folder_if_missing(arena, layout->distributions_path);
	BoldWhite("\n*-*-* Clear: %s  *-*-*\n", layout->package_path);
	fm_clear_folder(layout->package_path);
	BoldWhite("\n*-*-* Make if missing: %s  *-*-*\n", layout->package_path);
	fm_make_folder_if_missing(arena, layout->package_path);

	buildsuper(arena, project);
    build_main(arena, project, false);

	char *from, *to;

	from = compilation->platform_layer_out;
	to = fm_str(arena, layout->package_path, SLASH "4ed" EXE);
	BoldWhite("\n*-*-* Copying from: %s  to: %s *-*-*\n", from, to);
	fm_copy_file(from, to);

	from = compilation->app_target_out;
	to = fm_str(arena, layout->package_path, SLASH "4ed_app" DLL);
	BoldWhite("\n*-*-* Copying from: %s  to: %s *-*-*\n", from, to);
	fm_copy_file(from, to);

	from = compilation->custom_layer_out;
	to = fm_str(arena, layout->package_path, SLASH "custom_4coder" DLL);
	BoldWhite("\n*-*-* Copying from: %s  to: %s *-*-*\n", from, to);
	fm_copy_file(from, to);

	from = layout->dist_files_path;
	to = layout->package_path;
	BoldWhite("\n*-*-* Copying all from: %s  to: %s *-*-*\n", from, to);
	fm_copy_all(from, to);

	from = layout->ship_files_path;
	to = layout->package_path;
	BoldWhite("\n*-*-* Copying all from: %s  to: %s *-*-*\n", from, to);
	fm_copy_all(from, to);

	char *custom_src_dir = layout->custom_layer_path;
	char *custom_dst_dir = fm_str(arena, layout->package_path, SLASH, "custom");
	fm_make_folder_if_missing(arena, custom_dst_dir);
	fm_copy_all(custom_src_dir, custom_dst_dir);

	BoldWhite("\n*-*-* Make if missing: %s  *-*-*\n", layout->zip_path);
	fm_make_folder_if_missing(arena, layout->zip_path);
    fm_zip(layout->package_path, "4coder", layout->zip_name);
}

bool is_in_argv(char* argument, int argc, char** argv)
{
	bool is_in = false;

	String_Const_char arg_str = {argument, cstring_length(argument)};
	for(int idx = 0; idx < argc; ++idx)
	{
		String_Const_char current_arg = {argv[idx], cstring_length(argv[idx])};
		is_in = string_match(arg_str, current_arg);
	}

	return is_in;
}

char* get_custom_target(int argc, char** argv)
{
	char* custom_target = nullptr;

	char* custom_equal_str = "custom=";
	u64 custom_equal_length = cstring_length(custom_equal_str);
	String_Const_char custom_equal = {custom_equal_str, custom_equal_length};

	for(int i = 0; i < argc; ++i)
	{
		u64 argument_length = cstring_length(argv[i]);
		if (argument_length > custom_equal_length)
		{
			String_Const_char current_argument = {argv[i], custom_equal_length};
			bool is_custom_target_argument = string_match(current_argument, custom_equal);
			if (is_custom_target_argument)
			{
				custom_target = &argv[i][7];
			}
		}
	}

	return custom_target;
}


int main(int argc, char **argv){
    Arena arena = fm_init_system(DetailLevel_FileOperations);

	// INITIALIZE project
	char project_root[512];
	i32 n = fm_get_current_directory(project_root, sizeof(project_root));
    Assert(n < sizeof(project_root));

    u32 flags = SUPER;

	bool isDevelopmentBuild = is_in_argv("dev", argc, argv);
	bool isOptimizedBuild   = is_in_argv("opt", argc, argv);
	bool shouldPackage      = is_in_argv("package", argc, argv);

	if (isDevelopmentBuild) { flags |= DEBUG_INFO | INTERNAL; }
	if (isOptimizedBuild) { flags |= OPTIMIZATION; }
	if (shouldPackage) { flags |= OPTIMIZATION | SHIP; }

	Project project;

	Project::Layout& layout = project.layout;
	layout.project_root_path   = project_root,
	layout.core_layer_path     = fm_str(&arena, layout.project_root_path,  SLASH, "code");
	layout.non_source_path     = fm_str(&arena, layout.project_root_path,  SLASH, "non-source");
	layout.build_path          = fm_str(&arena, layout.project_root_path,  SLASH, "build");
	layout.custom_layer_path   = fm_str(&arena, layout.core_layer_path,    SLASH, "custom");
	layout.ship_files_path     = fm_str(&arena, layout.core_layer_path,    SLASH, "ship_files");
	layout.site_path           = fm_str(&arena, layout.core_layer_path,    SLASH, "site");
	layout.ship_themes_path    = fm_str(&arena, layout.ship_files_path,    SLASH, "themes");
	layout.dist_files_path     = fm_str(&arena, layout.non_source_path,    SLASH, "dist_files");
	layout.foreign_path        = fm_str(&arena, layout.non_source_path,    SLASH, "foreign");
	layout.build_themes_path   = fm_str(&arena, layout.build_path,         SLASH, "themes");
	layout.package_path        = fm_str(&arena, layout.build_path,         SLASH, "current_dist_super_x64");
	layout.distributions_path  = fm_str(&arena, layout.build_path,         SLASH, "distributions");
	layout.zip_path            = fm_str(&arena, layout.distributions_path, SLASH, "super_x64");
	layout.zip_name            = fm_str(&arena, layout.zip_path, SLASH, "4coder-" MAJOR_STR "-" MINOR_STR "-" PATCH_STR "-super-" OS_NAME "-x64" ".zip");

	Project::Compilation& compilation = project.compilation;
	compilation.app_target         = fm_str(&arena, layout.core_layer_path, SLASH "4ed_app_target.cpp");
	compilation.app_target_out     = fm_str(&arena, layout.build_path, SLASH, "4ed_app" DLL);
	compilation.platform_layer     = fm_str(&arena, layout.core_layer_path, SLASH, platform_layer_main_file);
	compilation.platform_layer_out = fm_str(&arena, layout.build_path, SLASH, "4ed" EXE);

	char* custom_target          = get_custom_target(argc, argv);
	compilation.custom_layer     = custom_target ? custom_target : fm_str(&arena, layout.custom_layer_path, SLASH, "4coder_default_bindings.cpp");
	compilation.custom_layer_out = fm_str(&arena, layout.build_path, SLASH, "custom_4coder" DLL);

	compilation.compiler_options     = compiler_flags;
	compilation.debug_options        = debug_flags;
	compilation.optimization_options = optimization_flags;
	compilation.arch_options         = arch_flags;

	compilation.exports      = fm_str(&arena, "app_get_functions");
	compilation.library      = libraries;
	compilation.icon         = icon_flags;
	compilation.so_includes  = fm_str(&arena, INCLUDE_FLAG, layout.custom_layer_path, INCLUDE_FLAG, layout.foreign_path, SLASH, "freetype2");
	compilation.bin_includes = fm_str(&arena, compilation.so_includes, INCLUDE_FLAG, "platform_unix");
	compilation.defines      = fm_str(&arena,
									  DEFINE_FLAG, HasFlag(flags, SHIP) ? "SHIP_MODE": "unused_flag",
									  DEFINE_FLAG, HasFlag(flags, INTERNAL) ? "FRED_INTERNAL": "unused_flag",
									  DEFINE_FLAG, HasFlag(flags, SUPER) ? "FRED_SUPER": "unused_flag"
									  );
	compilation.flags    = flags,
	compilation.os       = OS_NAME;
	compilation.compiler = COMPILER_NAME;

	if (!shouldPackage)
	{
		const bool shouldUpdateThemes = true;
		buildsuper(&arena, &project);
		build_main(&arena, &project, shouldUpdateThemes);
	}
	else
	{
		package(&arena, &project);
	}

    return(error_state);
}

// BOTTOM

