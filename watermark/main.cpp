#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


// 読み込み画像
//Mat src = imread("/Users/tsutsui/Desktop/C/watermark/watermark/watermark.png", 0);
//Mat mask_src = imread("/Users/tsutsui/Desktop/C/watermark/watermark/mask.png", 0);
//Mat mask2_src = imread("/Users/tsutsui/Desktop/C/watermark/watermark/mask2.png", 0);
Mat src = imread("/Users/daiji/Documents/C/watermark/watermark/watermark.png", 0);
Mat mask_src = imread("/Users/daiji/Documents/C/watermark/watermark/mask.png", 0);
Mat mask2_src = imread("/Users/daiji/Documents/C/watermark/watermark/mask2.png", 0);
// 結果画像
Mat *dst[8];
Mat mask(src.size(), src.type());
Mat mask2(src.size(), src.type());
Mat trg(src.size(), src.type());


int mono_intensity(int intensity, int mono_level){
	int bin_size = 256 / mono_level;
	return ((intensity / bin_size) % 2) * 255;
}

void split_mono_level(){
	int mono_level = 1;
		
	for(int i = 0; i < 8; i++){
		mono_level = 2 * mono_level;
		for(int y = 0; y < src.rows; ++y){
			for(int x = 0; x < src.cols; ++x){// 画素に直接アクセス
				dst[i]->data[ y * src.step + x * src.elemSize()] = mono_intensity(src.data[ y * src.step + x * src.elemSize()], mono_level);
			}
		}
		
		// 保存するファイルネームを作成
		ostringstream sout;
//        sout << "/Users/tsutsui/Desktop/C/watermark/watermark/lena_mono_" << i << ".png";
		sout << "/Users/daiji/Documents/C/watermark/watermark/lena_mono_" << i << ".png";
		imwrite(sout.str(), *dst[i]);
	}
}

void binary_mask(){
	for(int y = 0; y < src.rows; ++y){
		for(int x = 0; x < src.cols; ++x){// 画素に直接アクセス
			mask.data[ y * src.step + x * src.elemSize()] = mono_intensity(mask_src.data[ y * src.step + x * src.elemSize()], 2);
			mask2.data[ y * src.step + x * src.elemSize()] = mono_intensity(mask2_src.data[ y * src.step + x * src.elemSize()], 2);
		}
	}
	// 保存するファイルネームを作成
	ostringstream sout, sout2;
//    sout << "/Users/tsutsui/Desktop/C/watermark/watermark/mask_mono.png";
	sout << "/Users/daiji/Documents/C/watermark/watermark/mask_mono.png";
	imwrite(sout.str(), mask);
//    sout2 << "/Users/tsutsui/Desktop/C/watermark/watermark/mask2_mono.png";
	sout2 << "/Users/daiji/Documents/C/watermark/watermark/mask2_mono.png";
	imwrite(sout2.str(), mask2);
}

void watermark(){
	int mono_level = 256;
	
	for(int i = 0; i < 6; i++){
		mono_level = mono_level / 2;
		for(int y = 0; y < src.rows; ++y){
			for(int x = 0; x < src.cols; ++x){// 画素に直接アクセス
				trg.data[ y * src.step + x * src.elemSize()] += mono_level * (dst[i]->data[ y * src.step + x * src.elemSize()] / 255.0);
			}
		}
	}
	for(int y = 0; y < src.rows; ++y){
		for(int x = 0; x < src.cols; ++x){// 画素に直接アクセス
			trg.data[ y * src.step + x * src.elemSize()] += mask.data[ y * src.step + x * src.elemSize()] / 255.0;
			trg.data[ y * src.step + x * src.elemSize()] += 2.0* mask2.data[ y * src.step + x * src.elemSize()] / 255.0;
		}
	}
	// 保存するファイルネームを作成
	ostringstream sout;
//    sout << "/Users/tsutsui/Desktop/C/watermark/watermark/watermark.png";
	sout << "/Users/daiji/Documents/C/watermark/watermark/watermark.png";
	imwrite(sout.str(), trg);
	
	
}


int main(int argc, char *argv[])
{
	for(int i = 0; i < 8; i++){ dst[i] = new Mat(src.size(), src.type()); }
	split_mono_level();
	binary_mask();
	watermark();
	
	return 0;
}



