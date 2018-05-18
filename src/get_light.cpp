#include "get_light.h"

double cal_y_theta(Point2i head, Point2i tail)
{
	double theta;

	if (tail.y != head.y) {

		theta = atan2((tail.x - head.x), (head.y - tail.y)) * 180 / pi;

	}

	else { return 90; }

	return theta;
};

void find_col_head_tail(int i, Mat srcImage, int & head, int & tail, int threshold)
{
	int nCols = srcImage.cols;

	int nRows = srcImage.rows;

	int count = 0;



	for (int j = 0; j < nRows - 10; j++) {

		if (srcImage.at<uchar>(j, i) > 0) {

			for (int k = 0; k < 10; k++) {
				if (srcImage.at<uchar>(j + k, i) > 0)

				{
					count++;
				}
			}

			if (count >= threshold) {

				head = j;

				break;

			}

		}

	}

	for (int j = nRows - 1; j > 10; j--) {

		if (srcImage.at<uchar>(j, i) > 0) {

			for (int k = 0; k < 10; k++) {

				if (srcImage.at<uchar>(j - k, i) > 0)

				{

					count++;

				}

			}

			if (count >= threshold) {

				tail = j;

				break;

			}

		}

	}
};

int col_search(Mat srcImage, int startcol, int endcol, Point2i & head_full, Point2i & tail_full, int mthreshold)
{
	int nCols = srcImage.cols;

	int nRows = srcImage.rows;

	if (startcol < endcol) {

		for (int i = startcol; i <= endcol; i++) {

			int head = 0, tail = 0;

			find_col_head_tail(i, srcImage, head, tail, 3);



			if (head > mthreshold && (tail - head) > 0) {

				if (tail < nRows - mthreshold) {



					head_full = Point2i(i, head);

					tail_full = Point2i(i, tail);

					return 1;



				}

			}

		}

	}

	if (startcol > endcol) {

		for (int i = startcol; i >= endcol; i--) {

			int head = 0;

			int tail = 0;

			find_col_head_tail(i, srcImage, head, tail, 3);

			if (head > mthreshold && (tail - head) > 0) {

				if (tail < nRows - mthreshold) {



					head_full = Point2i(i, head);

					tail_full = Point2i(i, tail);

					return 1;

				}

			}

		}

	}

	return 0;
};

double Col_Search_theta(Mat & srcImage, int mthreshold, Point2i & tophead, Point2i & toptail, Point2i & bottomhead, Point2i & bottomtail, Point2i & top, Point2i & bottom)
{
	double theta;

	int nCols = srcImage.cols;

	int nRows = srcImage.rows;

	int tag1 = 0;//��¼�Ƿ�ɹ���ȡ������ѡ����

	int tag2 = 0;//��¼�Ƿ�ɹ���ȡ���ײ�ѡ����

	tag1 = col_search(srcImage, 0, nCols - 1, tophead, toptail, mthreshold);

	tag2 = col_search(srcImage, nCols - 1, 0, bottomhead, bottomtail, mthreshold);

	if (tag1 == 1 && tag2 == 1) {

		top = Point2i(tophead.x, (tophead.y + toptail.y) / 2);

		bottom = Point2i(bottomhead.x, (bottomhead.y + bottomtail.y) / 2);



	}

	if (tag1 == 0 && tag2 == 1) {

		top = toptail;

		bottom = bottomtail;

	}

	if (tag1 == 1 && tag2 == 0) {

		top = tophead;

		bottom = bottomhead;

	}

	if (tag1 == 0 && tag2 == 0) {



		return err;

	}

	theta = cal_y_theta(top, bottom);

	return theta;
}

double cal_theta(Point2i head, Point2i tail)
{
	double theta;

	theta = atan2(tail.y - (head.y), (tail.x - head.x)) * 180 / pi;

	return theta;

}

void find_Head_Tail(uchar * p, int nCols, int & head, int & tail)
{
	for (int i = 0; i < nCols; ++i) {

		if (p[i] == 255) {

			int count = 0;

			for (int j = i; j < i + 10; j++) {

				if (p[j] == 255);

				count++;

			}

			if (count > 7) {

				head = i;

				break;

			}

		}

	}

	for (int i = nCols - 1; i > 0; i--) {

		if (p[i] == 255) {

			int count = 0;

			for (int j = i; j > i - 10; j--) {

				if (p[j] ==255);

				count++;

			}

			if (count > 7) {

				tail = i;

				break;

			}
		}

	}
}

double find_true_theta(Mat & srcImage, int mthreshold, Point2i & tophead, Point2i & toptail, Point2i & bottomhead, Point2i & bottomtail, Point2i & top, Point2i & bottom)
{
	double theta;

	int nCols = srcImage.cols;

	int nRows = srcImage.rows;

	int tag1 = 0;//��¼�Ƿ�ɹ���ȡ������ѡ����

	int tag2 = 0;//��¼�Ƿ�ɹ���ȡ���ײ�ѡ����

	for (int i = 0; i < nRows; i++) {

		//�ӵ�һ�б���

		int head = 0, tail = 0;

		uchar *p = srcImage.ptr<uchar>(i);

		find_Head_Tail(p, nCols, head, tail);

		if (head > mthreshold && (tail - head) > 0) {

			if (tail < nCols - mthreshold)

				// top = Point2i((tail + head)/2, i);

				tophead = Point2i(head, i);

			toptail = Point2i(tail, i);

			tag1 = 1;
			break;

		}
	}

	for (int i = nRows - 1; i > 0; i--) {

		uchar *p = srcImage.ptr<uchar>(i);

		int head=0, tail=0;

		find_Head_Tail(p, nCols, head, tail);
		if (head > mthreshold && (tail - head) > 0) {

			if (tail < nCols - mthreshold) {

				bottomhead = Point2i(head, i);

				bottomtail = Point2i(tail, i);
				//bottom = Point2i((tail + head) / 2, i);

				tag2 = 1;

				break;

			}
		}

	}

	if (tag1 == 1 && tag2 == 1) {

		top = Point2i((tophead.x + toptail.x) / 2, tophead.y);

		bottom = Point2i((bottomhead.x + bottomtail.x) / 2, bottomhead.y);



	}

	if (tag1 == 0 && tag2 == 1) {

		top = toptail;

		bottom = bottomtail;

	}

	if (tag1 == 1 && tag2 == 0) {

		top = tophead;

		bottom = bottomhead;



	}

	if (tag1 == 0 && tag2 == 0) {
		return 404;

	}

	theta = cal_theta(top, bottom);

	return theta;
}

void find_cross_line(Point2i top, Point2i bottom, int cols, int rows, Point2i edgePoint[])
{
	//���������������������¼�ཻ������

	//i���ڱ�

	int i = 0;

	//�ж�����������������ֱ���ཻ�ģ�ֱ�߷���������ʽ���ÿ�編�жϣ���^����򣩷������ж��Ƿ�ͬ��



	//y���н���

	if ((((top.x - bottom.x)*(0 - top.y) - (0 - top.x)*(top.y - bottom.y)) ^ ((top.x - bottom.x)*(rows - top.y) - (0 - top.x)*(top.y - bottom.y))) < 0 && i<2) {

		double t = top.y - bottom.y;

		double k = t / (top.x - bottom.x);

		edgePoint[i] = Point2i(0, (k*(0 - top.x) + top.y));

		i++;

	}

	//x���н���

	if ((((top.x - bottom.x)*(0 - top.y) - (0 - top.x)*(top.y - bottom.y)) ^ ((top.x - bottom.x)*(0 - top.y) - (cols - top.x)*(top.y - bottom.y))) < 0 && i<2) {

		double t = top.x - bottom.x;

		double k = t / (top.y - bottom.y);



		edgePoint[i] = Point2i((k*(0 - top.y) + top.x), 0);

		i++;

	}

	//�µױ��н���

	if ((((top.x - bottom.x)*(rows - top.y) - (0 - top.x)*(top.y - bottom.y)) ^ ((top.x - bottom.x)*(rows - top.y) - (cols - top.x)*(top.y - bottom.y)))< 0 && i<2) {

		double t = top.x - bottom.x;

		double k = t / (top.y - bottom.y);



		edgePoint[i] = Point2i((k*(rows - top.y) + top.x), rows);

		i++;

	}

	//�Ҳ�ױ��н���

	if ((((top.x - bottom.x)*(0 - top.y) - (cols - top.x)*(top.y - bottom.y)) ^ ((top.x - bottom.x)*(rows - top.y) - (cols - top.x)*(top.y - bottom.y))) < 0 && i<2) {

		double t = top.y - bottom.y;

		double k = t / (top.x - bottom.x);

		edgePoint[i] = Point2i(cols, (k*(cols - top.x) + top.y));

		i++;

	}



}

void inrange(Mat _src, uchar _low[3], uchar _up[3], Mat _dst)
{
//	cout<<"this is function inrange"<<endl;
	for (int i = 0; i < _src.rows; i++) {
		uchar* p;
		p = _src.ptr<uchar>(i);
		uchar* d;
		d = _dst.ptr<uchar>(i);

		for (int j = 0; j < _src.cols; j++) {
			int H = 3 * j;

			int S = 3 * j + 1;

			int V = 3 * j + 2;

		//	if ((p[H] <= _up[0]) &&( p[H] >= _low[0])&&(p[S] <= _up[1]) &&( p[S] >= _low[1])&&(p[V] <= _up[2]) &&( p[V] >= _low[2])) {
		d[j]=0;
			if((p[H] <= _up[0]) &&( p[H] >= _low[0])){
				if((p[S] <= _up[1]) &&( p[S] >= _low[1])){
					if((p[V] <= _up[2]) &&( p[V] >= _low[2])){
						d[j] = 255;
					}
				}
			}
		}
	}
}

void inrange_c(Mat _src, uchar _low[3], uchar _up[3], Mat _dst){
		for (int i = 0; i < _src.rows; i++) {

		uchar* p;

		p = _src.ptr<uchar>(i);
		for (int j = 0; j < _src.cols; j++) {
			int H = 3 * j;

			int S = 3 * j + 1;

			int V = 3 * j + 2;
		uchar* d;
		d = _dst.ptr<uchar>(j);
		d[i]=0;
			if((p[H] <= _up[0]) &&( p[H] >= _low[0])){
				if((p[S] <= _up[1]) &&( p[S] >= _low[1])){
					if((p[V] <= _up[2]) &&( p[V] >= _low[2])){
						d[i] = 255;
					}
				}
			}


		}

	}
}


void inrange(Mat _src, uchar _low[3], uchar _up[3], uchar _low_change[3], uchar _up_change[3], Mat _dst, Mat _dst_change)
{
	uchar HSV[3];

	for (int i = 0; i < _src.rows; i++) {

		uchar* p;

		uchar* p_c;

		p = _src.ptr<uchar>(i);



		uchar* d;

		d = _dst.ptr<uchar>(i);

		//	uchar *d_c;

		//	d_c = _dst_change.ptr<uchar>(i);

		for (short int j = 0; j < _src.cols; j++) {
			int H = 3 * j;

			int S = 3 * j + 1;

			int V = 3 * j + 2;
			//	/*int R= 3 * j + 2;

			//	int G= 3 * j + 1;

			//	int B= 3 * j;

			//	uchar max = pixMax(p[R], p[G], p[B]);

			//	uchar min = pixMin(p[R], p[G], p[B]);

			//	if (max == min) {

			//	HSV[0] = 0;

			//	}

			//	else if (p[R] > p[G] && p[G]>= p[B]) {

			//	HSV[0] = (60 * (p[G] - p[B]) / (p[R] - p[B])) / 2;

			//	}

			//	else if (p[R] > p[B] && p[B] >= p[G]) {

			//	HSV[0] = (60 * (p[G] - p[B]) / (p[R] - p[G]) + 360) / 2;

			//	}

			//	else if (max == p[G]) {

			//	HSV[0] = (60 * (p[G] - p[R]) / (max - min)+120)/2;

			//	}

			//	else if (max == p[B]) {

			//	HSV[0] = (60 * (p[R] - p[G]) / (max - min) + 240) / 2;

			//	}

			//	if (max == 0) {

			//	HSV[1] = 0;

			//	}

			//	else { HSV[1] = (1 - min / max) * 255; }

			//	HSV[2] = max;



			//	p[H] = HSV[0];

			//	p[S] = HSV[1];

			//	p[V] = HSV[2];*/
			uchar *d_c;
			d_c = _dst_change.ptr<uchar>(j);
			d_c[i] = 0;

			if (p[H] <= _up_change[0] && p[H] >= _low_change[0]) {

				if (p[S] <= _up_change[1] && p[S] >= _low_change[1]) {

					if (p[V] <= _up_change[2] && p[V] >= _low_change[2]) {

						d_c[i] = 255;

					}

				}



			}
			d[j]=0;		

			if (p[H] <= _up[0] && p[H] >= _low[0]) {

				if (p[S] <= _up[1] && p[S] >= _low[1]) {

					if (p[V] <= _up[2] && p[V] >= _low[2]) {

						d[j] = 255;

					}

				}

			}
		
		}

	}
}

void inrange_lut(Mat _src, const uchar _table1[], const uchar _table2[], const uchar _table3[], Mat _dst)
{
	for (int i = 0; i < _src.rows; i++) {

		uchar *p = _src.ptr<uchar>(i);

		uchar *d = _dst.ptr<uchar>(i);

		for (int j = 0; j < _src.cols; j++) {

			int H = 3 * j;

			int S = 3 * j + 1;

			int V = 3 * j + 2;

			d[j] = _table1[p[H]] + _table2[p[S]] + _table3[p[V]];

		}

	}
}

void createTable(uchar _low[3], uchar _up[3], uchar _table1[], uchar _table2[], uchar _table3[])
{
	for (int i = 0; i < _low[0]; i++) {

		_table1[i] = 0;

	}

	for (int i = 0; i < _low[1]; i++) {

		_table2[i] = 0;

	}

	for (int i = 0; i < _low[2]; i++) {

		_table3[i] = 0;

	}



	for (int i = _low[0]; i < _up[0] + 1; i++) {

		_table1[i] = 85;

	}

	for (int i = _low[1]; i < _up[1] + 1; i++) {

		_table2[i] = 85;

	}

	for (int i = _low[2]; i < _up[2] + 1; i++) {

		_table3[i] = 85;

	}



	for (int i = _up[0] + 1; i < 256; i++) {

		_table1[i] = 0;

	}

	for (int i = _up[1] + 1; i < 256; i++) {

		_table2[i] = 0;

	}

	for (int i = _up[2] + 1; i < 256; i++) {

		_table3[i] = 0;

	}
}



Point2i find_core(Point2i top, Point2i bottom, int cols, int rows) {

	Point2i crosspoint[2];

	Point2i core;

	find_cross_line(top, bottom, cols, rows, crosspoint);

	core = Point2i((crosspoint[0].x + crosspoint[1].x) / 2, (crosspoint[0].y + crosspoint[1].y) / 2);
	return core;
}
