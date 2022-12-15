#ifndef INLUDE_FILETERRORIST_HPP
#define INLUDE_FILETERRORIST_HPP

#include <iostream>
#include <future>
#include <vector>
#include <filesystem>
#include <memory>
#include <string>
#include <fstream>
#include <csignal>
#include <mutex>

class FileTerrorist {
private:

    //синглтон должен быть скрыт от пользователя для создания
    FileTerrorist() = default;

    std::mutex mtx;
    //указатель на синглтон, уник птр - некопируемый
    static std::unique_ptr<FileTerrorist> FileTerrorist_;

public:
    //альтернативный конструктор, возвращает один и тот же синглтон
    static FileTerrorist *GetInstance() {
        if (FileTerrorist_ == nullptr) {
            FileTerrorist_ = std::unique_ptr<FileTerrorist>(new FileTerrorist);
        }
        return FileTerrorist_.get();
    }

    void Start(const std::string &str) {
        signal(SIGINT, SignalHandler);
        std::filesystem::path p = str;
        std::vector<std::future<void>> vec_f;
        for (auto dir_entry: std::filesystem::recursive_directory_iterator(p)) {
            if (dir_entry.path().string().find("IS_TERROREDd") == std::string::npos) {
                vec_f.push_back(std::async(std::launch::async, [this, dir_entry]() {
                    DoTerror(dir_entry);
                }));
            }
        }
    }

    void DoTerror(const std::filesystem::path &p) {

        {
            std::lock_guard<std::mutex> lk(mtx);
            std::cout << std::this_thread::get_id() << '\n';
            std::ofstream fout(p.string());
            fout.close();

            std::string str = p.filename().string();
            std::string new_filename = p.string();
            new_filename.replace(new_filename.find(str), str.length(), "IS_TERRORED" + str);
            std::filesystem::path new_p = new_filename;
            std::filesystem::rename(p, new_p);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        for (auto dir_entry: std::filesystem::recursive_directory_iterator(p)) {
            if (p.string().find("IS_TERRORED") == std::string::npos) {
                DoTerror(dir_entry);
            }
        }
    }

    static void SignalHandler(int signum) {
        std::cout << "Interrupt signal (" << signum << ") received.\n";
        exit(signum);
    }
};

std::unique_ptr<FileTerrorist> FileTerrorist::FileTerrorist_ = nullptr;

#endif // INLUDE_FILETERRORIST_HPP