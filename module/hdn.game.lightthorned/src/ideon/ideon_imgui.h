#pragma once

#include "imgui.h"

#include "core/core.h"
#include "pugixml/pugixml.hpp"

#include <filesystem>
#include <vector>

namespace hdn
{
	enum class TestExpressionType
	{
		Unknown,
		Check,
		Require
	};

	struct ModuleInfo
	{
		string name;
		u64 version;
		string author;
		string source;
		string semantic;
		string kind;
	};

	struct TestContext
	{
		std::string testExecutableName;
		u32 rngSeed;
	};

	struct OverallResult
	{
		bool success;
		f64 durationInSeconds;
	};

	struct OverallResults
	{
		u32 successes;
		u32 failures;
		u32 expectedFailures;
	};

	struct ExpressionResult
	{
		bool success;
		TestExpressionType type;
		std::filesystem::path filename;
		u32 line;
		std::string originalExpression;
		std::string expandedExpression;
	};

	struct SectionResult
	{
		std::string name;
		std::filesystem::path filename;
		u32 line;
		std::vector<ExpressionResult> expressionResults;
		OverallResults overallResults;
	};

	struct TestCaseResult
	{
		std::string name;
		std::string tags;
		std::filesystem::path filename;
		u32 line;
		std::vector<SectionResult> sectionResults;
		std::vector<ExpressionResult> expressionResults;
		OverallResult overallResult;
	};

	struct TestResult
	{
		TestContext context;
		std::vector<TestCaseResult> testCaseResults;
		OverallResults overallResultsExpressions;
		OverallResults overallResultsCases;
	};

	class IdeonImgui
	{
	public:
		static constexpr const char* ROOT_NODE_NAME = "Catch2TestRun";
		static constexpr const char* TEST_CASE_NODE_NAME = "TestCase";
		static constexpr const char* SECTION_NODE_NAME = "Section";
		static constexpr const char* EXPRESSION_NODE_NAME = "Expression";
		static constexpr const char* ORIGINAL_NODE_NAME = "Original";
		static constexpr const char* EXPANDED_NODE_NAME = "Expanded";
		static constexpr const char* BENCHMARK_NODE_NAME = "BenchmarkResults";
		static constexpr const char* OVERALL_RESULTS_NODE_NAME = "OverallResults";
		static constexpr const char* OVERALL_RESULTS_CASES_NODE_NAME = "OverallResultsCases";
		static constexpr const char* OVERALL_RESULT_NODE_NAME = "OverallResult";

		void ParseOverallResultNode(const pugi::xml_node& resultNode, OverallResult& overallResult);
		void ParseOverallResultsNode(const pugi::xml_node& resultNode, OverallResults& overallResults);
		void ParseSectionNode(const pugi::xml_node& sectionNode, SectionResult& sectionResult);
		void ParseExpressionNode(const pugi::xml_node& expressionNode, ExpressionResult& expressionResult);
		void ParseTestCaseNode(const pugi::xml_node& testCase, TestCaseResult& testCaseResult);
		void ParseRootNode(const pugi::xml_node& root, TestResult& out);
		void LoadTestResultFromMemory(const std::string& buffer, TestResult& testResult);
		void DisplayTestNode(const ExpressionResult& expression, ImGuiTreeNodeFlags treeNodeFlags);
		void DisplayTestNode(const SectionResult& section, ImGuiTreeNodeFlags treeNodeFlags);
		void DisplayTestNode(const TestCaseResult& testCase, ImGuiTreeNodeFlags treeNodeFlags);
		void DisplayTestNode(const TestResult& result, ImGuiTreeNodeFlags treeNodeFlags);
		void DisplayTestNode(const std::vector<TestResult>& results, ImGuiTreeNodeFlags treeNodeFlags);
		void ColoredTextIfValid(bool condition, ImVec4 color, int value);
		void SetRowColor(bool condition);
		void Draw();

	private:
		std::thread m_WaitThread;
		TVector<TestResult> m_TestResults;
		TVector<ModuleInfo> m_ModuleInfo;
		bool m_RunningTests = false;
		bool m_RanTestsAtLeastOneTime = false;
	};
}