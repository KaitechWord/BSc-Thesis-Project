#pragma once
#include "../ImageFilter/ImageFilter.h"
#include <vector>

class SmartImageFilter : public ImageFilter {

struct Indices
{
	struct MaskIndices
	{
		int left;
		int right;
		int center;
	} firstMask, secondMask;
	int row;
	int col;
	
};
using Precalculation = std::vector<int>;

struct Precalculations{
	Precalculation prefixes;
	Precalculation suffixes;
};

public:
	SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize);
	void apply(cv::Mat& image) override;
private:
	void filter(cv::Mat& newImage, int firstIndex, int lastIndex);

	int getIndexPrefixColumn( const Indices& indices, int left, int right, int row );
	int getIndexSuffixColumn( const Indices& indices, int left, int right, int row );

	void calculatePrefixesColumn( const Indices& indices, Precalculation& precalculation, int row );

	void calculateSuffixesColumn( const Indices& indices, Precalculation& precalculation, int row );

	void precalculateColumn( const Indices& indices, Precalculations& precalculations );

	void setPrefixOnlyMaskExtremumColumn( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );

	void setAffixMixMaskExtremaColumn( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );

	void setSuffixOnlyMaskExtremumColumn( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );

	void setExtremaColumn( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );

	void updateRowColumnAndIndexColumn( int& row, int& column, int& index );

	void updateIndicesColumn( Indices& indices, int row, int column );

	void updateRowColumnAndIndicesColumn( int& row, int& column, int& index, Indices& indices );

	// ROW

	void updateIndicesRow( Indices& indices, int row, int column );
	int getIndexPrefixRow( const Indices& indices, int left, int right, int col );
	int getIndexSuffixRow( const Indices& indices, int left, int right, int col );
	void calculatePrefixesRow( cv::Mat& indirect, const Indices& indices, Precalculation& precalculation, int col );
	void calculateSuffixesRow( cv::Mat& indirect, const Indices& indices, Precalculation& precalculation, int col );
	void setPrefixOnlyMaskExtremumRow( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );
	void setAffixMixMaskExtremaRow( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );
	void setSuffixOnlyMaskExtremumRow( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );
	void setExtremaRow( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );
	bool updateRowColumnRow( int& row, int& column, int lastRow, int lastCol );
};
