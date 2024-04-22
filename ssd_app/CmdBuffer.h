#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include "iCommand.h"

class CmdBuffer
{
public:
	CmdBuffer(std::string file_name) :
		file_name_(file_name) {}

	bool isFull();
	uint32_t GetSize();
	void SetSize(uint32_t length);
	void Clear();
	void AddCmd(iCmd* cmd);
	void GetCmd(std::vector<std::string>& cmds);
	void GetCmdList(std::vector<std::vector<std::string>>& cmds);
	void GetiCmdList(std::vector<iCmd*>& cmds);
	bool FastRead(uint32_t read_addr, uint32_t& read_val);
	std::vector<iCmd*> GetOptimizedCmds();

private:
	void CreateEmptyFile();
	std::fstream OpenFile(bool isTrunc = false);
	std::vector<std::string> SplitStringToVector(const std::string& str);
	void InsertWriteCmd(std::map<uint32_t, uint32_t>& m, iCmd* cmd);
	void InsertEraseCmd(std::map<uint32_t, uint32_t>& m, iCmd* cmd);
	std::map<uint32_t, uint32_t> CreateDataMap(const std::vector<iCmd*>& cmds);
	bool IsOverWritten(const uint32_t addr, iCmd* cmd);
	bool IsErased(const uint32_t addr, iCmd* cmd);
	bool isRemovableWriteCmd(const std::vector<iCmd*>& cmds, const int cmd_idx);
	std::vector<iCmd*> OptimizeWriteCmds(const std::vector<iCmd*>& cmds);
	void AddEraseArea(std::set<int>& erase_addrs, iCmd* cmd);
	void InsertEraseCmd(std::vector<iCmd*>& erase_cmds, int start, int last);
	std::vector<iCmd*> GetMergedEraseCmds(std::set<int>& erase_addrs);
	std::vector<iCmd*> OptimizeEraseCmds(const std::vector<iCmd*>& cmds);
	std::vector<iCmd*> OptimizeCmds(std::vector<iCmd*>& cmds);

	uint32_t length_;
	const uint32_t MAX_BUFFER_SIZE = 10;
	std::string file_name_;
};
