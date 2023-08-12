
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

struct Layout
{
    char* project_root_path;
    char* core_layer_path;
    char* custom_layer_path;
    char* build_path;
    char* ship_files_path;
    char* docs_path;
    char* languages_path;
    char* site_path;
    char* non_source_path;
    char* foreign_path;
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
    char* app_target_exports;
    char* custom_layer_exports;
    char* library;
    char* icon;
    
    char* so_includes;
    char* bin_includes;
    
    u32 flags;
    char* os;
    char* compiler;
};

struct Project
{
	Layout layout;
	Compilation compilation;
};

#define ExitIfError(error) if(error){ printf("ERROR: error-id(%d)\n", error); exit(error);}

#if OS_WINDOWS
char* platform_layer_main_file = "platform_win32" SLASH "win32_4ed.cpp";
#elif OS_LINUX
char* platform_layer_main_file = "platform_linux" SLASH "linux_4ed.cpp";
#elif OS_MAC
char* platform_layer_main_file = "platform_mac"   SLASH "mac_4ed.mm";
#endif


#if COMPILER_CL
# define compiler_flags "-W4 -wd4310 -wd4100 -wd4201 -wd4505 -wd4996 -wd4127 -wd4510 -wd4512 -wd4610 -wd4390 -wd4611 -wd4189 -wd4457 -wd4389 -WX -GR- -EHa- -nologo -FC "
// -link -INCREMENTAL:NO -RELEASE -PDBALTPATH:%%_PDB%%"
# define debug_flags " -Zi -DDO_CRAZY_EXPENSIVE_ASSERTS"
# define optimization_flags " -O2"
# define arch_flags " -DFTECH_64_BIT"
# define libraries "user32.lib winmm.lib gdi32.lib opengl32.lib comdlg32.lib userenv.lib .\\non-source\\foreign\\x64\\freetype.lib .\\non-source\\foreign\\x86\\freetype.lib "
# define app_target_export_flags " -OPT:REF -EXPORT:app_get_functions"
# define custom_layer_export_flags " /EXPORT:get_version /EXPORT:init_apis "
# define icon_flags ".\\non-source\\res\\icon.res"
# define SHARED_FLAG "  -LD "
# define REMOVE_PROGRAM "del "
# define PREPROCESS_FLAG " /E"
# define PREPROCESS_OUT_FLAG " > "
# define SHARED_OUT_FLAG " /link -INCREMENTAL:NO -RELEASE -PDBALTPATH:%%_PDB%% /DLL /OUT:"
# define OUT_FLAG " /Fe"
# define INCLUDE_FLAG " -I"
# define DEFINE_FLAG " -D"

#elif COMPILER_GCC
# define compiler_flags "-Wno-write-strings -D_GNU_SOURCE -fPIC -fno-threadsafe-statics -pthread -Wno-unused-result -std=c++11"
# define debug_flags "-g3 -ggdb3 -fno-eliminate-unused-debug-types -fvar-tracking -fno-eliminate-unused-debug-symbols"
# define optimization_flags " -O3"
# define arch_flags " -m64 -DFTECH_64_BIT"
# define app_target_export_flags " "
# define custom_layer_export_flags " "
# define icon_flags " "
# define SHARED_FLAG " -shared -fPIC"
# define REMOVE_PROGRAM "rm "
# define PREPROCESS_FLAG " -E"
# define PREPROCESS_OUT_FLAG " -o "
# define SHARED_OUT_FLAG SHARED_FLAG " -o "
# define OUT_FLAG " -o "
# define INCLUDE_FLAG " -I"
# define DEFINE_FLAG " -D"

# if OS_LINUX
#  define libraries "-lX11 -lpthread -lm -lrt -lGL -ldl -lXfixes -lfreetype -lfontconfig"
# else
#  error gcc options not set for this platform
# endif

#elif COMPILER_CLANG
# define compiler_flags "-Wno-write-strings -Wno-null-dereference -Wno-switch -Wno-missing-declarations -Wno-format-extra-args -D_GNU_SOURCE -fPIC -fno-threadsafe-statics -pthread -Wno-unused-result -std=c++11"
# define debug_flags "-g3 -ggdb3 -fno-eliminate-unused-debug-types "
# define optimization_flags " -O3"
# define arch_flags " -m64 -DFTECH_64_BIT"
# define app_target_export_flags " "
# define custom_layer_export_flags " "
# define icon_flags " "
# define SHARED_FLAG " -shared -fPIC"
# define REMOVE_PROGRAM "rm "
# define PREPROCESS_FLAG " -E"
# define PREPROCESS_OUT_FLAG " -o "
# define SHARED_OUT_FLAG SHARED_FLAG " -o "
# define OUT_FLAG " -o "
# define INCLUDE_FLAG " -I"
# define DEFINE_FLAG " -D"

# if OS_MAC
#  define libraries "-framework Cocoa -framework QuartzCore -framework CoreServices -framework OpenGL -framework IOKit -framework Metal -framework MetalKit ./non-source/foreign/x86/libfreetype-mac.a ./non-source/foreign/x64/libfreetype-mac.a"
# elif OS_LINUX
#  define libraries "-lX11 -lpthread -lm -lrt -lGL -ldl -lXfixes -lfreetype -lfontconfig"
# else
#  error clang options not set for this platform
# endif

#elif COMPILER_CIRCLE
# define compiler_flags "-Wno-c++11-narrowing -Wno-writable-strings -Wno-deprecated-declarations -Wno-comment -Wno-switch -Wno-null-dereference  -Wno-tautological-compare  -std=gnu++11"
# define debug_flags "-g "
# define optimization_flags " -O3"
# define arch_flags " -m64 -DFTECH_64_BIT"
# define app_target_export_flags " "
# define custom_layer_export_flags " "
# define icon_flags " "
# define SHARED_FLAG " --shared -fPIC"
# define REMOVE_PROGRAM "rm "
# define PREPROCESS_FLAG " -E"
# define PREPROCESS_OUT_FLAG " > "
# define SHARED_OUT_FLAG SHARED_FLAG " -o "
# define OUT_FLAG " -o "
# define INCLUDE_FLAG " -I"
# define DEFINE_FLAG " -D"

# if OS_LINUX
#  define libraries " -l=X11 -l=pthread -l=m -l=rt -l=GL -l=dl -l=Xfixes -l=freetype -l=fontconfig"
# else
#  error circle options not set for this platform
# endif

#else
# error build defineds not specified for this compiler
#endif

internal void
build_super(Arena *arena, const Project* project)
{
	const Layout* layout =  &project->layout ;
	const Compilation* compilation =  &project->compilation;
    
	printf("\n*-*-* Generate the preproc file for the metadata generator *-*-*\n");
	char* include_home_folder = fm_str(arena, INCLUDE_FLAG, layout->custom_layer_path);
	char* source = compilation->custom_layer;
	char* meta_macros = DEFINE_FLAG "META_PASS";
	char* arch_flag = compilation->arch_options;
	char* defines = compilation->defines;
	char* opts = compilation->compiler_options;
	char* debug = HasFlag(compilation->flags, DEBUG_INFO) ? compilation->debug_options : (char*)"";
    char* optimization = HasFlag(compilation->flags, OPTIMIZATION) ? compilation->optimization_options : (char*)"";
    char* preproc_file = fm_str(arena, layout->custom_layer_path, SLASH, "4coder_command_metadata.i ");
	systemf("%s %s %s %s %s %s %s %s %s %s %s%s",
			compilation->compiler,
			include_home_folder,
			meta_macros,
			arch_flag,
			defines,
			opts,
			debug,
            optimization,
			source,
			PREPROCESS_FLAG,
			PREPROCESS_OUT_FLAG,
			preproc_file);
	ExitIfError(error_state);
    
	printf("\n*-*-* Build the metadata generator *-*-*\n");
	char* metadata_generator_cpp = fm_str(arena, layout->custom_layer_path, SLASH, "4coder_metadata_generator.cpp");
	char* metadata_generator     = fm_str(arena, layout->custom_layer_path, SLASH, "metadata_generator");
	systemf("%s %s %s %s %s %s %s %s%s",
			compilation->compiler,
			include_home_folder,
			defines,
			opts,
			debug,
            optimization,
			metadata_generator_cpp,
			OUT_FLAG,
			metadata_generator);
	ExitIfError(error_state);
    
	printf("\n*-*-* Generate meta data *-*-*\n");
	char* home_folder = layout->custom_layer_path;
	systemf("%s -R %s %s",
			metadata_generator,
			home_folder,
			preproc_file);
	ExitIfError(error_state);
    
	printf("\n*-*-* Build the custom layer: %s *-*-*\n", source);
	char* shared_library = compilation->custom_layer_out;
	systemf("%s %s %s %s %s %s %s %s %s %s%s %s",
			compilation->compiler,
			include_home_folder,
			arch_flag,
			opts,
			debug,
            optimization,
            defines,
			SHARED_FLAG,
			source,
			SHARED_OUT_FLAG,
			shared_library,
			compilation->custom_layer_exports);
	ExitIfError(error_state);
    
	printf("\n*-*-* Clear temporary files *-*-*\n");
#if OS_WINDOWS
    systemf("%s %s.exe", REMOVE_PROGRAM, metadata_generator);
    systemf("%s %s.ilk", REMOVE_PROGRAM, metadata_generator);
	systemf("%s %s.pdb", REMOVE_PROGRAM, metadata_generator);
#else
	systemf("%s %s", REMOVE_PROGRAM, metadata_generator);
#endif
    ExitIfError(error_state);
    
    systemf("%s %s", REMOVE_PROGRAM, preproc_file);
	ExitIfError(error_state);
}

internal void
build_cleanup(const Project* project)
{
    
#if OS_WINDOWS
    printf("\n*-*-* Clear temporary files *-*-\n");
    systemf("%s *.obj", REMOVE_PROGRAM);
    systemf("%s *.exp", REMOVE_PROGRAM);
    systemf("%s *.lib", REMOVE_PROGRAM);
    systemf("%s *.pdb", REMOVE_PROGRAM);
#endif
    
}

internal void
build_shared(const Project* project)
{
    const Layout* layout = &project->layout;
    const Compilation* compilation = &project->compilation;
    
	systemf("%s %s %s %s %s%s %s%s %s %s    %s %s %s%s %s",
            compilation->compiler,
            compilation->arch_options,
            compilation->compiler_options,
            compilation->defines,
            INCLUDE_FLAG, layout->core_layer_path,
            INCLUDE_FLAG, layout->custom_layer_path,
            HasFlag(compilation->flags, DEBUG_INFO) ? compilation->debug_options : "",
            HasFlag(compilation->flags, OPTIMIZATION) ? compilation->optimization_options : "",
            compilation->so_includes,
            compilation->app_target,
            SHARED_OUT_FLAG,
            compilation->app_target_out,
            compilation->app_target_exports
            );
    
	ExitIfError(error_state);
}

internal void
build_binary(const Project* project)
{
    const Layout* layout = &project->layout;
    const Compilation* compilation = &project->compilation;
    
	systemf("%s %s %s %s %s%s %s%s %s %s   %s %s %s %s %s%s",
            compilation->compiler,
            compilation->arch_options,
            compilation->compiler_options,
            compilation->defines,
            INCLUDE_FLAG, layout->core_layer_path,
            INCLUDE_FLAG, layout->custom_layer_path,
            HasFlag(compilation->flags, DEBUG_INFO) ? compilation->debug_options : "",
            HasFlag(compilation->flags, OPTIMIZATION) ? compilation->optimization_options : "",
            compilation->bin_includes,
            compilation->library,
            compilation->icon,
            compilation->platform_layer,
            OUT_FLAG,
            compilation->platform_layer_out
            );
    
	ExitIfError(error_state);
}

internal void
build_file(const Project* project, char* file_path, char* file_out_path)
{
    const Layout* layout = &project->layout;
    const Compilation* compilation = &project->compilation;
    
    printf("\n*-*-* Building file %s: output %s *-*-\n", file_path, file_out_path);
	systemf("%s %s %s %s %s%s %s%s %s %s   %s %s %s %s %s%s",
            compilation->compiler,
            compilation->arch_options,
            compilation->compiler_options,
            compilation->defines,
            INCLUDE_FLAG, layout->core_layer_path,
            INCLUDE_FLAG, layout->custom_layer_path,
            HasFlag(compilation->flags, DEBUG_INFO) ? compilation->debug_options : "",
            HasFlag(compilation->flags, OPTIMIZATION) ? compilation->optimization_options : "",
            compilation->bin_includes,
            compilation->library,
            compilation->icon,
            file_path,
            OUT_FLAG,
            file_out_path
            );
    
	ExitIfError(error_state);
}

internal void
run_file(char* file)
{
    systemf("%s", file);
    ExitIfError(error_state);
}

internal void
build_main(Arena *arena, const Project* project, b32 update_local_assets)
{
	// Build the 4ed_app - shared library
	printf("\n*-*-* Build the 4ed library (%s -> %s) *-*-*\n", project->compilation.app_target, project->compilation.app_target_out);
	build_shared(project);
    
	// Build the 4ed binary
	printf("\n*-*-* Build the 4ed binary (%s -> %s) *-*-*\n", project->compilation.platform_layer, project->compilation.platform_layer_out);
	build_binary(project);
    
    char* cpp_lexer_gen = "4coder_cpp_lexer_gen";
    char* cpp_lexer_gen_cpp = fm_str(arena, project->layout.languages_path, SLASH, cpp_lexer_gen, CPP);
    char* cpp_lexer_gen_exe = fm_str(arena, project->layout.build_path, SLASH, cpp_lexer_gen, EXE);
    build_file(project, cpp_lexer_gen_cpp, cpp_lexer_gen_exe);
    run_file(cpp_lexer_gen_exe);
    
	// Clean up temporary files (windows...)
    build_cleanup(project);
    
    if (update_local_assets)
	{
        char* ship_files_folder = project->layout.ship_files_path;
        char* build_folder = project->layout.build_path;
        
		printf("\n*-*-* Copying all from: %s  to: %s *-*-*\n", ship_files_folder, build_folder);
		fm_copy_all(ship_files_folder, build_folder);
    }
}

internal void
build_docs(Arena* arena, Project* project)
{
    Layout* layout = &project->layout;
    Compilation* compilation = &project->compilation;
    
    //char* custom_api_main = "4ed_doc_custom_api_main";
    //char* custom_api_main_cpp = fm_str(arena, layout->docs_path, SLASH, custom_api_main, CPP);
    //char* custom_api_main_exe = fm_str(arena, layout->build_path, SLASH, custom_api_main, EXE);
    //build_file(project, custom_api_main_cpp, custom_api_main_exe);
    //run_file(custom_api_main_exe);
    
    
    char* token_tester = "4coder_cpp_lexer_test";
    char* token_tester_cpp = fm_str(arena, layout->languages_path, SLASH, token_tester, CPP);
    char* token_tester_exe = fm_str(arena, layout->build_path, SLASH, token_tester, EXE);
    build_file(project, token_tester_cpp, token_tester_exe);
    run_file(token_tester_exe);
    
    //char* system_api = "4ed_system_api";
    //char* system_api_cpp = fm_str(arena, layout->core_layer_path, SLASH, system_api, CPP);
    //char* system_api_exe = fm_str(arena, layout->build_path, SLASH, system_api, EXE);
    //build_file(project, system_api_cpp, system_api_exe);
    //run_file(system_api_exe);
    
    //char* font_api = "4ed_font_api";
    //char* font_api_cpp = fm_str(arena, layout->core_layer_path, SLASH, font_api, CPP);
    //char* font_api_exe = fm_str(arena, layout->build_path, SLASH, font_api, EXE);
    //build_file(project, font_api_cpp, font_api_exe);
    //run_file(font_api_exe);
    
    //char* graphics_api = "4ed_graphics_api";
    //char* graphics_api_cpp = fm_str(arena, layout->core_layer_path, SLASH, graphics_api, CPP);
    //char* graphics_api_exe = fm_str(arena, layout->build_path, SLASH, graphics_api, EXE);
    //build_file(project, graphics_api_cpp, graphics_api_exe);
    //run_file(graphics_api_exe);
    
    //char* keycodes_generator = "4ed_generate_keycodes";
    //char* keycodes_generator_cpp = fm_str(arena, layout->core_layer_path, SLASH, keycodes_generator, CPP);
    //char* keycodes_generator_exe = fm_str(arena, layout->build_path, SLASH, keycodes_generator, EXE);
    //build_file(project, keycodes_generator_cpp, keycodes_generator_exe);
    //run_file(keycodes_generator_exe);
    
    //char* site_render = "4ed_site_render_main";
    //char* site_render_cpp = fm_str(arena, layout->site_path, SLASH, site_render, CPP);
    //char* site_render_exe = fm_str(arena, layout->build_path, SLASH, site_render, EXE);
    //build_file(project, site_render_cpp, site_render_exe);
    
    //char* api_parser_main_cpp = fm_str(arena, layout->core_layer_path, SLASH, "4ed_api_parser_main", CPP);
    //char* api_parser_main_exe = fm_str(arena, layout->build_path, SLASH,"api_parser", EXE);
    //build_file(project, api_parser_main_cpp, api_parser_main_exe);
    
    //char* api_check_cpp = fm_str(arena, layout->core_layer_path, SLASH, "4ed_api_check", CPP);
    //char* api_check_exe = fm_str(arena, layout->build_path, SLASH, "api_checker", EXE);
    //build_file(project, api_check_cpp, api_check_exe);
    
    //systemf("%s %s", api_parser_main_exe, "4ed_api_implementation.cpp");
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
    bool buildDocs          = is_in_argv("docs", argc, argv);
    
	if (isDevelopmentBuild) { flags |= DEBUG_INFO | INTERNAL; }
	if (isOptimizedBuild) { flags |= OPTIMIZATION | SHIP; }
    
	Project project;
    
	Layout& layout = project.layout;
	layout.project_root_path   = project_root,
	layout.core_layer_path     = fm_str(&arena, layout.project_root_path,  SLASH, "code");
	layout.non_source_path     = fm_str(&arena, layout.project_root_path,  SLASH, "non-source");
	layout.build_path          = fm_str(&arena, layout.project_root_path,  SLASH, "build");
	layout.custom_layer_path   = fm_str(&arena, layout.core_layer_path,    SLASH, "custom");
	layout.ship_files_path     = fm_str(&arena, layout.core_layer_path,    SLASH, "ship_files");
	layout.docs_path           = fm_str(&arena, layout.core_layer_path,    SLASH, "docs");
    layout.languages_path      = fm_str(&arena, layout.custom_layer_path,  SLASH, "languages");
    layout.site_path           = fm_str(&arena, layout.core_layer_path,    SLASH, "site");
	layout.foreign_path        = fm_str(&arena, layout.non_source_path,    SLASH, "foreign");
    
	Compilation& compilation = project.compilation;
	compilation.app_target         = fm_str(&arena, layout.core_layer_path, SLASH "4ed_app_target.cpp");
	compilation.app_target_out     = fm_str(&arena, layout.build_path, SLASH, "4ed_app" DLL);
	compilation.platform_layer     = fm_str(&arena, layout.core_layer_path, SLASH, platform_layer_main_file);
	compilation.platform_layer_out = fm_str(&arena, layout.build_path, SLASH, "4ed" EXE);
    
	char* custom_target          = get_custom_target(argc, argv);
	compilation.custom_layer     = custom_target ? fm_str(&arena, layout.custom_layer_path, SLASH, custom_target, SLASH, custom_target, ".cpp") : fm_str(&arena, layout.custom_layer_path, SLASH, "4coder_default_bindings.cpp");
	compilation.custom_layer_out = fm_str(&arena, layout.build_path, SLASH, "custom_4coder" DLL);
    
	compilation.compiler_options     = (char*)compiler_flags;
	compilation.debug_options        = (char*)debug_flags;
	compilation.optimization_options = (char*)optimization_flags;
	compilation.arch_options         = (char*)arch_flags;
    
	compilation.app_target_exports   = (char*)app_target_export_flags;
	compilation.custom_layer_exports = (char*)custom_layer_export_flags;
	compilation.library              = (char*)libraries;
	compilation.icon                 = (char*)icon_flags;
	compilation.so_includes          = fm_str(&arena, INCLUDE_FLAG, layout.custom_layer_path, INCLUDE_FLAG, layout.foreign_path, SLASH, "freetype2");
	compilation.bin_includes         = fm_str(&arena, compilation.so_includes, INCLUDE_FLAG, "platform_unix");
	compilation.defines              = fm_str(&arena,
                                              DEFINE_FLAG, HasFlag(flags, SHIP) ? "SHIP_MODE": "unused_flag",
                                              DEFINE_FLAG, HasFlag(flags, INTERNAL) ? "FRED_INTERNAL": "unused_flag",
                                              DEFINE_FLAG, HasFlag(flags, SUPER) ? "FRED_SUPER": "unused_flag",
											  DEFINE_FLAG, "OS_WINDOWS=", OS_WINDOWS?"1":"0",
											  DEFINE_FLAG, "OS_LINUX=", OS_LINUX?"1":"0",
											  DEFINE_FLAG, "OS_MAC=", OS_MAC?"1":"0"
                                              );
	compilation.flags    = flags,
	compilation.os       = (char*)OS_NAME;
	compilation.compiler = (char*)COMPILER_NAME;
    
    if (buildDocs)
    {
        build_docs(&arena, &project);
    }
    else
	{
		const bool shouldUpdateThemes = !isDevelopmentBuild;
		
		printf("\n*-*-* Make if missing: %s  *-*-*\n", layout.build_path);
		fm_make_folder_if_missing(&arena, layout.build_path);
        
        build_super(&arena, &project);
		build_main(&arena, &project, shouldUpdateThemes);
	}
    
    return(error_state);
}

// BOTTOM