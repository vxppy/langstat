#pragma once

#include <string_view>
#include <cstdint>

enum class Language;

struct ExtEntry {
    const char* ext;
    Language language;
};

struct LanguageInfo {
    const char* key;
    const char* name;

    uint32_t color;
};

enum class Language {
    Unknown = 0,

    C,
    CPP,
    Rust,
    Python,
    JavaScript,
    TypeScript,
    Java,
    Kotlin,
    Go,
    CSharp,
    Swift,
    PHP,
    Ruby,
    Lua,
    Zig,
    Odin,
    Nim,
    Dart,
    Scala,
    Haskell,
    OCaml,
    R,
    Julia,
    Perl,
    Shell,
    PowerShell,

    HTML,
    CSS,
    SCSS,
    JSON,
    YAML,
    TOML,
    XML,
    Markdown,

    SQL,

    COUNT

};


inline constexpr ExtEntry ext_map[] {
    // C / C++
    { ".c", Language::C },
    { ".h", Language::C },

    { ".cpp", Language::CPP },
    { ".cc", Language::CPP },
    { ".cxx", Language::CPP },
    { ".hpp", Language::CPP },
    { ".hh", Language::CPP },
    { ".hxx", Language::CPP },

    // Rust
    { ".rs", Language::Rust },

    // Python
    { ".py", Language::Python },

    // JavaScript / TypeScript
    { ".js", Language::JavaScript },
    { ".mjs", Language::JavaScript },
    { ".cjs", Language::JavaScript },

    { ".ts", Language::TypeScript },
    { ".mts", Language::TypeScript },
    { ".cts", Language::TypeScript },
    { ".tsx", Language::TypeScript },

    // Java / Kotlin
    { ".java", Language::Java },
    { ".kt", Language::Kotlin },
    { ".kts", Language::Kotlin },

    // Go
    { ".go", Language::Go },

    // C#
    { ".cs", Language::CSharp },

    // Swift
    { ".swift", Language::Swift },

    // PHP
    { ".php", Language::PHP },

    // Ruby
    { ".rb", Language::Ruby },

    // Lua
    { ".lua", Language::Lua },

    // Zig
    { ".zig", Language::Zig },

    // Odin
    { ".odin", Language::Odin },

    // Nim
    { ".nim", Language::Nim },

    // Dart
    { ".dart", Language::Dart },

    // Scala
    { ".scala", Language::Scala },

    // Haskell
    { ".hs", Language::Haskell },

    // OCaml
    { ".ml", Language::OCaml },
    { ".mli", Language::OCaml },

    // R
    { ".r", Language::R },

    // Julia
    { ".jl", Language::Julia },

    // Perl
    { ".pl", Language::Perl },

    // Shell
    { ".sh", Language::Shell },
    { ".bash", Language::Shell },
    { ".zsh", Language::Shell },

    // PowerShell
    { ".ps1", Language::PowerShell },

    // Web / Data
    { ".html", Language::HTML },
    { ".htm", Language::HTML },

    { ".css", Language::CSS },
    { ".scss", Language::SCSS },

    { ".json", Language::JSON },

    { ".yaml", Language::YAML },
    { ".yml", Language::YAML },

    { ".toml", Language::TOML },

    { ".xml", Language::XML },

    { ".md", Language::Markdown },

    // SQL
    { ".sql", Language::SQL },
};


inline constexpr LanguageInfo language_info[] {
    // Unknown
    {
        .key = "unknown",
        .name = "Unknown",
        .color = 0x6B7280
    },

    // C
    {
        .key = "c",
        .name = "C",
        .color = 0xA8B9CC
    },

    // C++
    {
        .key = "cpp",
        .name = "C++",
        .color = 0x00599C
    },

    // Rust
    {
        .key = "rust",
        .name = "Rust",
        .color = 0xDEA584
    },

    // Python
    {
        .key = "python",
        .name = "Python",
        .color = 0x3776AB
    },

    // JavaScript
    {
        .key = "javascript",
        .name = "JavaScript",
        .color = 0xF7DF1E
    },

    // TypeScript
    {
        .key = "typescript",
        .name = "TypeScript",
        .color = 0x3178C6
    },

    // Java
    {
        .key = "java",
        .name = "Java",
        .color = 0xED8B00
    },

    // Kotlin
    {
        .key = "kotlin",
        .name = "Kotlin",
        .color = 0x7F52FF
    },

    // Go
    {
        .key = "go",
        .name = "Go",
        .color = 0x00ADD8
    },

    // C#
    {
        .key = "csharp",
        .name = "C#",
        .color = 0x239120
    },

    // Swift
    {
        .key = "swift",
        .name = "Swift",
        .color = 0xFA7343
    },

    // PHP
    {
        .key = "php",
        .name = "PHP",
        .color = 0x777BB4
    },

    // Ruby
    {
        .key = "ruby",
        .name = "Ruby",
        .color = 0xCC342D
    },

    // Lua
    {
        .key = "lua",
        .name = "Lua",
        .color = 0x2C2D72
    },

    // Zig
    {
        .key = "zig",
        .name = "Zig",
        .color = 0xF7A41D
    },

    // Odin
    {
        .key = "odin",
        .name = "Odin",
        .color = 0x3882D2
    },

    // Nim
    {
        .key = "nim",
        .name = "Nim",
        .color = 0xFFE953
    },

    // Dart
    {
        .key = "dart",
        .name = "Dart",
        .color = 0x0175C2
    },

    // Scala
    {
        .key = "scala",
        .name = "Scala",
        .color = 0xDC322F
    },

    // Haskell
    {
        .key = "haskell",
        .name = "Haskell",
        .color = 0x5D4F85
    },

    // OCaml
    {
        .key = "ocaml",
        .name = "OCaml",
        .color = 0xEC6813
    },

    // R
    {
        .key = "r",
        .name = "R",
        .color = 0x276DC3
    },

    // Julia
    {
        .key = "julia",
        .name = "Julia",
        .color = 0x9558B2
    },

    // Perl
    {
        .key = "perl",
        .name = "Perl",
        .color = 0x39457E
    },

    // Shell
    {
        .key = "shell",
        .name = "Shell",
        .color = 0x89E051
    },

    // PowerShell
    {
        .key = "powershell",
        .name = "PowerShell",
        .color = 0x5391FE
    },

    // HTML
    {
        .key = "html",
        .name = "HTML",
        .color = 0xE34F26
    },

    // CSS
    {
        .key = "css",
        .name = "CSS",
        .color = 0x1572B6
    },

    // SCSS
    {
        .key = "scss",
        .name = "SCSS",
        .color = 0xCF649A
    },

    // JSON
    {
        .key = "json",
        .name = "JSON",
        .color = 0xF1E05A
    },

    // YAML
    {
        .key = "yaml",
        .name = "YAML",
        .color = 0xCB171E
    },

    // TOML
    {
        .key = "toml",
        .name = "TOML",
        .color = 0x9C4221
    },

    // XML
    {
        .key = "xml",
        .name = "XML",
        .color = 0xFF6600
    },

    // Markdown
    {
        .key = "markdown",
        .name = "Markdown",
        .color = 0x083FA1
    },

    // SQL
    {
        .key = "sql",
        .name = "SQL",
        .color = 0x336791
    },
};

const Language get_language_from_ext(std::string_view ext);
