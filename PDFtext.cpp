#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <PDF/PDFNet.h>
#include <PDF/PDFDoc.h>
#include <PDF/TextExtractor.h>

using namespace std;
using namespace pdftron;
using namespace PDF;
using namespace Common;

int main(int argc, char* argv[]) {
	int ret = 0;
    //创建并打开输出文件
	ofstream outfile("out.txt");
	if (!outfile) {
		cout << "Unable to open outfile";
		exit(1);
	} //打开失败退出
	//设置读取的PDF文件地址和名称，这里你可以改成别的路径、文件名，此处我设置的是当前目录叫test.pdf的文件
	string input_path = "test.pdf";
    const char* filein = argc > 1 ? argv[1] : input_path.c_str();
	//设置输出模式，由于我定义了两种输出，一种是普通输出，一种是XML格式输出，因此这里可以自定义，如果都为true就会把两种输出都输出到输出文件里
	bool example1_basic = true;
	bool example2_xml = false;
	//读取PDF文件并输出
	try {
		PDFDoc doc(filein);
		doc.InitSecurityHandler();
		int page_count = doc.GetPageCount();
		for (int i = 1; i <= page_count; i++) {
			Page page = doc.GetPage(i);
			if (!page) {
				cout << "Page not found.\n" << endl;
				return 1;
			} //该页不存在
			TextExtractor txt;
			txt.Begin(page); //该页存在，故读取页面
			//输出模式1，将每一页所有文本存在一个字符串里，单词之间以空格或换行符隔开
			if (example1_basic) {
				//outfile << "Word Count: " << txt.GetWordCount() << endl; //统计每页单词数
				UString text;
				txt.GetAsText(text);
				outfile << text << endl;
			}
			//输出模式2，输出XML
			if (example2_xml) {
				UString text;
				txt.GetAsXML(text, TextExtractor::e_words_as_elements | TextExtractor::e_output_bbox | TextExtractor::e_output_style_info);
				outfile << text << endl;
			}
		}
	}
	catch (Exception & e) {
		cout << e << endl;
		ret = 1;
	}
	catch (...) {
		cout << "Unknown Exception" << endl;
		ret = 1;
	}
	PDFNet::Terminate();
	return ret;
}
