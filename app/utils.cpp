#include <utils.hpp>
#include <argparse.hpp>
#include <fmt/format.h>
#include <fmt/color.h>

namespace detail {

  // RAII method of restoring a buffer
  struct buffer_restorer {
      std::ios &m_s;
      std::streambuf *m_buf;

      buffer_restorer(std::ios &s, std::streambuf *buf) : m_s(s), m_buf(buf) {}
      ~buffer_restorer() { m_s.rdbuf(m_buf); }
  };

  std::ifstream fin;
  std::ofstream ferr, fout;
  buffer_restorer r_cin(std::cin, std::cin.rdbuf());
  buffer_restorer r_cerr(std::cerr, std::cerr.rdbuf());
  buffer_restorer r_cout(std::cout, std::cout.rdbuf());

} // namespace detail

void utils::open_file(std::ifstream &inFile, const std::string &fname) {
    inFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
        inFile.open(DATA_PATH + fname);

    } catch (const std::ifstream::failure &e) {
        try {
            inFile.open(fname);

        } catch (const std::ifstream::failure &e) {
            std::cerr << "Error: Could not open " << fname << std::endl;
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    inFile.exceptions(std::ifstream::badbit);
}

utils::args utils::parse_args_and_redirect_streams(int argc, char *argv[]) {
    argparse::ArgumentParser program("./app", "2.0");

    program.add_argument("-i", "--inputf")
      .help("specify the input file");

    program.add_argument("-o", "--outputf")
      .help("specify the output file");

    program.add_argument("-l", "--logf")
      .help("specify the log file");

    program.add_argument("-v", "--verbose")
      .default_value(false)
      .implicit_value(true)
      .help("write useful debug statements that describe the state at every stage");

    program.add_argument("-nc", "--nocolor")
      .default_value(false)
      .implicit_value(true)
      .help("disable color printing");

    try {
        program.parse_args(argc, argv);
        if(program.get<bool>("--verbose") == false and program.present("--logf"))
            throw std::runtime_error("--verbose must be set to true");
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    args params = { "", "", "", false, true };

    params.enable_color = !program.get<bool>("--nocolor");
    params.verbose = program.get<bool>("--verbose");
    if(params.verbose)
        std::cout << "Running in verbose mode" << std::endl;

    if(auto p = program.present("--inputf")) {
        fmt::print("> {} stream specified:  {} (file)\n",
            format_col(params.enable_color, fg(fmt::color::gold), "input"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "{}", *p));

        params.inputf = *p;
        utils::open_file(detail::fin, *p);
        std::cin.rdbuf(detail::fin.rdbuf());
    } else {
        fmt::print("> {} stream specified:  {}\n",
            format_col(params.enable_color, fg(fmt::color::gold), "input"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "stdin"));
    }

    if(auto p = program.present("--logf")) {
        fmt::print("> {} stream specified:    {} (file)\n",
            format_col(params.enable_color, fg(fmt::color::gold), "log"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "{}", *p));

        params.logf = *p;
        detail::ferr.open(*p, std::ios::trunc);
        std::cerr.rdbuf(detail::ferr.rdbuf());
    } else if(params.verbose) {
        fmt::print("> {} stream specified:    {}\n",
            format_col(params.enable_color, fg(fmt::color::gold), "log"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "stderr"));
    }

    if(auto p = program.present("--outputf")) {
        fmt::print("> {} stream specified: {} (file)\n",
            format_col(params.enable_color, fg(fmt::color::gold), "output"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "{}", *p));

        params.outputf = *p;
        detail::fout.open(*p, std::ios::trunc);
        std::cout.rdbuf(detail::fout.rdbuf());
    } else {
        fmt::print("> {} stream specified: {}\n",
            format_col(params.enable_color, fg(fmt::color::gold), "output"),
            format_col(params.enable_color, fg(fmt::color::yellow_green), "stdout"));
    }

    fmt::print("\n");

    return params;
}