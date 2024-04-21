#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "iCommand.h"
#include "Reader.h"
#include "Writer.h"
#include "Eraser.h"

class CmdFactory
{
public:
	iCmd* CreateCmd(const std::string& cmd)
	{
        std::string mode;
        std::vector<std::string> args;

        splitString(cmd, mode, args);
        if (mode == "R")
        {
            return new ReadCmd(args);
        }
        else if (mode == "W")
        {
            return new WriteCmd(args);
        }
        else if (mode == "E")
        {
            return new EraseCmd(args);
        }
        else
        {
            throw std::exception("Invalid mode");
        }
	}

    iCmd* CreateCmd(const std::string& mode, std::vector<std::string>& args)
    {
        if (mode == "R")
        {
            return new ReadCmd(args);
        }
        else if (mode == "W")
        {
            return new WriteCmd(args);
        }
        else if (mode == "E")
        {
            return new EraseCmd(args);
        }
        else
        {
            throw std::exception("Invalid mode");
        }
    }

private:
    void splitString(const std::string& input, std::string& mode,
                    std::vector<std::string>& args) {
        std::stringstream ss(input);
        ss >> mode;

        std::string word;
        while (ss >> word) {
            args.push_back(word);
        }
    }
};