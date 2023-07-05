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

typedef u32 Tier_Code;
enum
{
	Tier_Demo,
	Tier_Super,

	Tier_Count,
	Tier_None = Tier_Count
};

char* tier_names[]=
{
	"demo",
	"super"
};

typedef u32 Platform_Code;
enum{
    Platform_Windows,
    Platform_Linux,
    Platform_Mac,
    //
    Platform_COUNT,
    Platform_None = Platform_COUNT,
};

char *platform_names[] = {
    "win",
    "linux",
    "mac",
};

typedef u32 Compiler_Code;
enum{
    Compiler_CL,
    Compiler_GCC,
    Compiler_Clang,
	Compiler_Circle,
	//
	Compiler_COUNT,
    Compiler_None = Compiler_COUNT,
};

char *compiler_names[] = {
    "cl",
    "g++",
    "clang",
    "circle",
};

#if OS_WINDOWS
# define This_OS Platform_Windows
#elif OS_LINUX
# define This_OS Platform_Linux
#elif OS_MAC
# define This_OS Platform_Mac
#else
# error This platform is not enumerated.
#endif

#if COMPILER_CL
# define This_Compiler Compiler_CL
#elif COMPILER_GCC
# define This_Compiler Compiler_GCC
#elif COMPILER_CLANG
# define This_Compiler Compiler_Clang
#elif COMPILER_CIRCLE
# define This_Compiler Compiler_Circle
#else
# error This compilers is not enumerated.
#endif

struct Project
{
	// folders
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
	} layout;

	struct Compilation
	{
		// files
		char* app_target;
		char* app_target_out;
		char* platform_layer;
		char* platform_layer_out;
		char* custom_layer;
		char* custom_layer_out;

		// compiler options
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

		// flags
		u32 flags;
		//u32 arch;
		u32 os_id;
		char* os;
		u32 compiler_id;
		char* compiler;
	}compilation;
};


char* win_layer[] = {"platform_win32/win32_4ed.cpp", 0};
char* lin_layer[] = {"platform_linux/linux_4ed.cpp", 0};
char* mac_layer[] = {"platform_mac/mac_4ed.mm", 0};

char** platform_layers[Platform_COUNT] = {
	/*win*/win_layer,
	/*lin*/lin_layer,
	/*mac*/mac_layer,
};

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

#if COMPILER_CL

#define CL_OPTS                                  \
"-W4 -wd4310 -wd4100 -wd4201 -wd4505 -wd4996 "  \
"-wd4127 -wd4510 -wd4512 -wd4610 -wd4390 "      \
"-wd4611 -wd4189 -WX -GR- -EHa- -nologo -FC -link -INCREMENTAL:NO -RELEASE -PDBALTPATH:%%_PDB%%"

#define FOREIGN_WIN "..\\non-source\\foreign"

#define CL_LIBS_COMMON \
"user32.lib winmm.lib gdi32.lib opengl32.lib comdlg32.lib userenv.lib "
#define CL_LIBS_X64 CL_LIBS_COMMON FOREIGN_WIN "\\x64\\freetype.lib"
#define CL_LIBS_X86 CL_LIBS_COMMON FOREIGN_WIN "\\x86\\freetype.lib"

#define CL_ICON "..\\non-source\\res\\icon.res"

#define compiler_flags CL_OPTS
#define debug_flags " -Zi -DDO_CRAZY_EXPENSIVE_ASSERTS"
#define optimization_flags " -O2"
#define arch_flags " -MACHINE:X64 -DFTECH_64_BIT"
#define libraries CL_LIBS_COMMON FOREIGN_WIN "\\x64\\freetype.lib " " -NODEFAULTLIB:library"
#define exports " -OPT:REF -EXPORT:"
#define icon_flags CL_ICON
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

//    if (flags & OPTS){
//        fm_add_to_line(line, CL_OPTS);
//    }

//    switch (arch){
//        case Arch_X64: fm_add_to_line(line, "-DFTECH_64_BIT"); break;
//        case Arch_X86: fm_add_to_line(line, "-DFTECH_32_BIT"); break;
//        default: InvalidPath;
//    }

//    fm_add_to_line(line, "-I%s", code_path);
//    if (inc_folders != 0){
//        for (u32 i = 0; inc_folders[i] != 0; ++i){
//            char *str = fm_str(arena, code_path, "/", inc_folders[i]);
//            fm_add_to_line(line, "-I%s", str);
//        }
//    }

//    if (flags & LIBS){
//        switch (arch){
//            case Arch_X64: fm_add_to_line(line, CL_LIBS_X64); break;
//            case Arch_X86: fm_add_to_line(line, CL_LIBS_X86); break;
//            default: InvalidPath;
//        }
//    }

//    if (flags & ICON){
//        fm_add_to_line(line, CL_ICON);
//    }

//    if (flags & DEBUG_INFO){
//        fm_add_to_line(line, "-Zi");
//        fm_add_to_line(line, "-DDO_CRAZY_EXPENSIVE_ASSERTS");
//    }

//    if (flags & OPTIMIZATION){
//        fm_add_to_line(line, "-O2");
//    }

//    if (flags & SHARED_CODE){
//        fm_add_to_line(line, "-LD");
//    }

//    if (defines != 0){
//        for (u32 i = 0; defines[i] != 0; ++i){
//            char *define_flag = fm_str(arena, "-D", defines[i]);
//            fm_add_to_line(line, "%s", define_flag);
//        }
//    }

//    for (u32 i = 0; code_files[i]; ++i){
//        fm_add_to_line(line, "\"%s\"", code_files[i]);
//    }

//    fm_add_to_line(line, "-Fe%s", out_file);

//    fm_add_to_line(line, "-link -INCREMENTAL:NO -RELEASE -PDBALTPATH:%%_PDB%%");
//    switch (arch){
//        case Arch_X64: fm_add_to_line(line, "-MACHINE:X64"); break;
//        case Arch_X86: fm_add_to_line(line, "-MACHINE:X86"); break;
//        default: InvalidPath;
//    }

//    if (flags & DEBUG_INFO){
//        fm_add_to_line(line, "-DEBUG");
//    }

//    if (flags & SHARED_CODE){
//        Assert(exports != 0);
//        fm_add_to_line(line, "-OPT:REF");
//        for (u32 i = 0; exports[i] != 0; ++i){
//            char *str = fm_str(arena, "-EXPORT:", exports[i]);
//            fm_add_to_line(line, "%s", str);
//        }
//    }
//    else{
//        fm_add_to_line(line, "-NODEFAULTLIB:library");
//    }

//    fm_finish_build_line(&line);

//    //printf("%s\n", line.build_options);
//    systemf("%s", line.build_options);
//    fm_popdir(temp);

//    fflush(stdout);
//}

#elif COMPILER_GCC

#if OS_LINUX

# define GCC_OPTS                      \
"-Wno-write-strings "                 \
"-D_GNU_SOURCE -fPIC "                \
"-fno-threadsafe-statics -pthread "   \
"-Wno-unused-result "                 \
"-std=c++11"

# define GCC_LIBS_COMMON       \
"-lX11 -lpthread -lm -lrt "   \
"-lGL -ldl -lXfixes -lfreetype -lfontconfig"

#define compiler_flags GCC_OPTS
#define debug_flags "-g3 -ggdb3 -O0 -fno-eliminate-unused-debug-types -fvar-tracking -fno-eliminate-unused-debug-symbols"
#define optimization_flags " -O3"
#define arch_flags " -m64 -DFTECH_64_BIT"
#define libraries GCC_LIBS_COMMON
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

# define CLANG_OPTS \
"-Wno-write-strings -Wno-deprecated-declarations " \
"-Wno-comment -Wno-switch -Wno-null-dereference " \
"-Wno-tautological-compare -Wno-unused-result " \
"-Wno-missing-declarations -Wno-nullability-completeness " \
"-std=c++11 -v"

#define CLANG_LIBS_COMMON \
"-framework Cocoa -framework QuartzCore " \
"-framework CoreServices " \
"-framework OpenGL -framework IOKit -framework Metal -framework MetalKit "

#define FOREIGN "./non-source/foreign"

#define CLANG_LIBS_X64 CLANG_LIBS_COMMON \
FOREIGN "/x64/libfreetype-mac.a"

#define CLANG_LIBS_X86 CLANG_LIBS_COMMON \
FOREIGN "/x86/libfreetype-mac.a"

#define compiler_flags CLANG_OPTS
#define debug_flags "-g3 -O0 -fno-eliminate-unused-debug-types "
#define optimization_flags " -O3"
#define arch_flags " -m64 -DFTECH_64_BIT"
#define libraries CLANG_LIBS_COMMON FOREIGN "/x64/libfreetype-mac.a"
#define icon_flags " "
#define SHARED_FLAG " -shared -fPIC"
#define REMOVE_PROGRAM "rm "
#define PREPROCESS_FLAG " -E"
#define OUT_FLAG " -o "
#define INCLUDE_FLAG " -I"
#define DEFINE_FLAG " -D"

#else
# error clang options not set for this platform
#endif

#elif COMPILER_CIRCLE

# if OS_LINUX
#  define CIRCLE_OPTS \
"-Wno-writable-strings -Wno-deprecated-declarations " \
"-Wno-comment -Wno-switch -Wno-null-dereference " \
"-Wno-tautological-compare  " \
"-std=c++11"

#  define CIRCLE_LIBS_COMMON       \
"-lX11 -lpthread -lm -lrt "   \
"-lGL -ldl -lXfixes -lfreetype -lfontconfig"

#  define compiler_flags CIRCLE_OPTS
#  define debug_flags "-g -O0  -fvar-tracking -fno-eliminate-unused-debug-symbols"
#  define optimization_flags " -O3"
#  define arch_flags " -m64 -DFTECH_64_BIT"
#  define libraries CIRCLE_LIBS_COMMON
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
buildsuper(Arena *arena, const Project::Layout* layout, const Project::Compilation* compilation)
{
	// GENERATE THE PREPROC FILE FOR THE METADATA GENERATOR
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

	// BUILD THE METADATA GENERATOR
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

	// GENERATE THE METADATA
	char* home_folder = layout->custom_layer_path;
	systemf("%s -R %s %s",
			metadata_generator,
			home_folder,
			preproc_file);

	// BUDILD THE CUSTOM LAYER
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

	// REMOVE THE METADATA GENERATOR
	systemf("%s %s",
			REMOVE_PROGRAM,
			metadata_generator);

	// REMOVE THE PREPROC FILE
	systemf("%s %s",
			REMOVE_PROGRAM,
			preproc_file);
	fflush(stdout);
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
build_main(Arena *arena, const Project* project, b32 update_local_theme){

	u32 flags = project->compilation.flags;

	// Build the 4ed_app - shared library
	build(arena, SHARED_CODE | flags, &project->layout, &project->compilation);

	// Build the 4ed binary
	build(arena, LIBS | flags, &project->layout, &project->compilation);

    if (update_local_theme){
        char *themes_folder = project->layout.build_themes_path;
        char *source_themes_folder = project->layout.ship_themes_path;
        fm_clear_folder(themes_folder);
        fm_make_folder_if_missing(arena, themes_folder);
        fm_copy_all(source_themes_folder, themes_folder);
    }

    fflush(stdout);
}

internal char*
get_4coder_dist_name(Arena *arena, u32 platform, char *tier, u32 arch){
    char *name = fm_str(arena, "4coder-" MAJOR_STR "-" MINOR_STR "-" PATCH_STR "-", tier);
    if (platform != Platform_None){
        name = fm_str(arena, name, "-", platform_names[platform]);
    }

	name = fm_str(arena, name, "-x64");

    return(name);
}

function void
package_for_arch(Arena *arena, u32 arch, char *cdir, char *build_dir, char *pack_dir, i32 tier, char *tier_name,  char *current_dist_tier, u32 flags, char** dist_files, i32 dist_file_count){
    char *arch_name  = "x64";
    char *parent_dir = fm_str(arena, current_dist_tier, "_", arch_name);
    char *dir        = fm_str(arena, parent_dir, SLASH "4coder");
    char *zip_dir    = fm_str(arena, pack_dir, SLASH, tier_name, "_", arch_name);

    printf("\nBUILD: %s_%s\n", tier_name, arch_name);
    printf(" parent_dir = %s;\n", parent_dir);
    printf(" dir = %s;\n", dir);
    printf(" zip_dir = %s;\n", zip_dir);
    fflush(stdout);

	// UPDATE THIS CALL TO USE THE NEW PROJECT_LAYOUT STRUCT
    //buildsuper(arena, cdir /*this should be project root*/, cdir /*this should be custom layer path*/, fm_str(arena, default_custom_target), arch);
    //build_main(arena, cdir /*this should be project root*/, cdir /*this should be custom layer path*/, false, flags, arch);

    fm_clear_folder(parent_dir);
    fm_make_folder_if_missing(arena, parent_dir);

    fm_make_folder_if_missing(arena, dir);
    fm_copy_file(fm_str(arena, build_dir, "/4ed" EXE), fm_str(arena, dir, "/4ed" EXE));
    fm_copy_file(fm_str(arena, build_dir, "/4ed_app" DLL), fm_str(arena, dir, "/4ed_app" DLL));
    fm_copy_file(fm_str(arena, build_dir, "/custom_4coder" DLL), fm_str(arena, dir, "/custom_4coder" DLL));

    if (tier == Tier_Demo){
        dist_file_count -= 1;
    }

    for (i32 j = 0; j < dist_file_count; j += 1){
        fm_copy_all(dist_files[j], dir);
    }

    if (tier == Tier_Super){
        char *custom_src_dir = fm_str(arena, cdir, SLASH, "custom");
        char *custom_dst_dir = fm_str(arena, dir, SLASH, "custom");
        fm_make_folder_if_missing(arena, custom_dst_dir);
        fm_copy_all(custom_src_dir, custom_dst_dir);
    }

    char *dist_name = get_4coder_dist_name(arena, This_OS, tier_name, 0);
    char *zip_name = fm_str(arena, zip_dir, SLASH, dist_name, ".zip");
    fm_make_folder_if_missing(arena, zip_dir);
    fm_zip(parent_dir, "4coder", zip_name);
}

internal u32
tier_flags(Tier_Code code){
    u32 result = 0;
    switch (code){
        case Tier_Super:
        {
            result = SUPER;
        }break;
    }
    return(result);
}

internal void
package(Arena *arena, char *cdir, Tier_Code tier, u32 arch){
    // NOTE(allen): meta
    char *build_dir = fm_str(arena, "./build");
    char *pack_dir = fm_str(arena, "./distributions");
    char *dist_files[2];
    dist_files[0] = fm_str(arena, "non-source/dist_files");
    dist_files[1] = fm_str(arena, "ship_files");

    printf("build dir: %s\n", build_dir);
    printf("pack dir: %s\n", pack_dir);
    printf("dist files: %s, %s\n", dist_files[0], dist_files[1]);
    fflush(stdout);

    u32 base_flags = SHIP | DEBUG_INFO | OPTIMIZATION;

    fm_make_folder_if_missing(arena, pack_dir);

    char *tier_name = tier_names[tier];
    u32 flags = base_flags | tier_flags(tier);
    Temp_Memory temp = begin_temp(arena);
    char *current_dist_tier = fm_str(arena, ".." SLASH "current_dist_super");//, tier_name);
    package_for_arch(arena, arch, cdir, build_dir, pack_dir, tier, tier_name, current_dist_tier, flags, dist_files, ArrayCount(dist_files));
    end_temp(temp);
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
	u64 custom_equal_length = cstring_length(const_cast<char*>(custom_equal_str));
	String_Const_char custom_equal = {custom_equal_str, custom_equal_length};

	for(int i = 0; i < argc; ++i)
	{
		u64 argument_length = cstring_length(const_cast<char*>(argv[i]));
		if (argument_length > custom_equal_length)
		{
			String_Const_char current_argument = {const_cast<char*>(argv[i]), custom_equal_length};
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
	Tier_Code tier = Tier_Super;

	bool isDevelopmentBuild = is_in_argv("dev", argc, argv);
	bool isOptimizedBuild   = is_in_argv("opt", argc, argv);
	//bool isX86Arch          = is_in_argv("x86", argc, argv) ;
	bool shouldPackage      = is_in_argv("package", argc, argv);

	if (isDevelopmentBuild) { flags |= DEBUG_INFO | INTERNAL; }
	if (isOptimizedBuild) { flags |= OPTIMIZATION; }
	//if (isX86Arch) { arch = Arch_X86; }


	char* core_layer_path = fm_str(&arena, project_root, SLASH, "code");
	char* custom_layer_path = fm_str(&arena, core_layer_path, SLASH, "custom");
	char* build_path = fm_str(&arena, project_root, SLASH, "build");
	char* ship_files_path = fm_str(&arena, core_layer_path, SLASH, "ship_files");
	char* ship_themes_path = fm_str(&arena, ship_files_path, SLASH, "themes");
	char* build_themes_path = fm_str(&arena, build_path, SLASH, "themes");
	char* site_path = fm_str(&arena, core_layer_path, SLASH, "site");
	char* distributions_path = fm_str(&arena, project_root, SLASH, "distributions");
	char* non_source_path = fm_str(&arena, project_root, SLASH, "non-source");
	char* dist_files_path =  fm_str(&arena, non_source_path, SLASH, "dist_files");
	char* foreign_path =  fm_str(&arena, non_source_path, SLASH, "foreign");

	char* app_target = fm_str(&arena, core_layer_path, SLASH "4ed_app_target.cpp");
	char* app_target_out = fm_str(&arena, build_path, SLASH, "4ed_app" DLL);
	char* platform_layer = fm_str(&arena, core_layer_path, SLASH, platform_layers[This_OS][0]);
	char* platform_layer_out = fm_str(&arena, build_path, SLASH, "4ed" EXE);
	char* custom_layer = fm_str(&arena, custom_layer_path, SLASH, "4coder_default_bindings.cpp");
	char* custom_layer_out = fm_str(&arena, build_path, SLASH, "custom_4coder" DLL);

	char* platform_compiler_options = compiler_flags;
	char* platform_debug_options = debug_flags;
	char* platform_optimization_options = optimization_flags;
	char* platform_arch_options = arch_flags;
	char* platform_library_list = libraries;
	char* platform_icon = icon_flags;

	char* defines_str = fm_str(&arena,
							   DEFINE_FLAG, HasFlag(flags, SHIP) ? "SHIP_MODE": "unused_flag",
							   DEFINE_FLAG, HasFlag(flags, INTERNAL) ? "FRED_INTERNAL": "unused_flag",
							   DEFINE_FLAG, HasFlag(flags, SUPER) ? "FRED_SUPER": "unused_flag"
							   );

	char* platform_defines_list = defines_str;
	char* platform_exports_list = fm_list_one_item(&arena, "app_get_functions")[0];

	char* so_includes = fm_str(&arena, INCLUDE_FLAG, custom_layer_path, INCLUDE_FLAG, foreign_path, SLASH, "freetype2");
	char* bin_includes = fm_str(&arena, so_includes, INCLUDE_FLAG, "platform_unix");

	u32 os_id = This_OS;
	char* os = platform_names[This_OS];
	u32 compiler_id = This_Compiler;
	char* compiler = compiler_names[This_Compiler];

	char* custom_target = get_custom_target(argc, argv);
	if (custom_target) { custom_layer = custom_target; }

	const Project project =
	{
		.layout =
		{
			.project_root_path = project_root,
			.core_layer_path = core_layer_path,
			.custom_layer_path = custom_layer_path,
			.build_path = build_path,
			.ship_files_path = ship_files_path,
			.ship_themes_path = ship_themes_path,
			.build_themes_path = build_themes_path,
			.site_path = site_path,
			.distributions_path = distributions_path,
			.non_source_path = non_source_path,
			.dist_files_path = dist_files_path,
			.foreign_path = foreign_path,
		},
		.compilation =
		{
			.app_target = app_target,
			.app_target_out = app_target_out,
			.platform_layer = platform_layer,
			.platform_layer_out = platform_layer_out,
			.custom_layer = custom_layer,
			.custom_layer_out = custom_layer_out,

			.compiler_options = platform_compiler_options,
			.debug_options = platform_debug_options,
			.optimization_options = platform_optimization_options,
			.arch_options = platform_arch_options,

			.defines = platform_defines_list,
			.exports = platform_exports_list,
			.library = platform_library_list,
			.icon = platform_icon,

			.so_includes = so_includes,
			.bin_includes = bin_includes,

			//FLAGS
			.flags = flags,
			//.arch = arch,
			.os_id = os_id,
			.os = os,
			.compiler_id = compiler_id,
			.compiler = compiler,
		}
	};

	if (!shouldPackage)
	{
		const bool shouldUpdateThemes = true;
		buildsuper(&arena, &project.layout, &project.compilation);
		build_main(&arena, &project, shouldUpdateThemes);
	}
	else
	{
		package(&arena, custom_layer_path, tier, 0);
	}

    return(error_state);
}

// BOTTOM

