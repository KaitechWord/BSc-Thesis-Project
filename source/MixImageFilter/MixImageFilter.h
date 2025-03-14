#pragma once
#include "../ImageFilter/ImageFilter.h"
#include <vector>

class MixImageFilter : public ImageFilter {

struct Indices
{
	std::vector< int > lastExtremaRow;
	struct MaskIndices
	{
		int left;
		int right;
		int center;
	} firstMask, secondMask;
	int top;
	int bot;
	int row;
	int rowToBeOverriden;
	
};
using Precalculation = std::vector<int>;

struct Precalculations{
	Precalculation prefixes;
	Precalculation suffixes;
};

public:
	MixImageFilter(int threadNum, AlgorithmType algType, int maskSize);
	void apply(cv::Mat& image) override;
private:
	void filter(cv::Mat& newImage, int firstIndex, int lastIndex);

	int getIndexPrefix( const Indices& indices, int left, int right, int row );
	int getIndexSuffix( const Indices& indices, int left, int right, int row );

	void calculatePrefixes( const Indices& indices, Precalculation& precalculation, int row );

	void calculateSuffixes( const Indices& indices, Precalculation& precalculation, int row );

	void precalculate( const Indices& indices, Precalculations& precalculations );
	void precalculateOnlyLast( const Indices& indices, Precalculations& precalculations );

	void setPrefixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );

	void setAffixMixMaskExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );

	void setSuffixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );

	void setExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );

	void updateRowColumnAndIndex( int& row, int& column, int& index );

	void updateIndices( Indices& indices, int row, int column );



	//FRESH
	void updateRowColumnAndIndices( int& row, int& column, int& index, Indices& indices );
	void setFreshExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );
	void setFreshSuffixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );
	void setFreshAffixMixMaskExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations );
	void setFreshPrefixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation );
	int getIndexSuffixFresh(const Indices& indices, int left, int right, int row);
	int getIndexPrefixFresh(const Indices& indices, int left, int right, int row);
	int getIndexSuffixFromRowToBeOverriden(const Indices& indices, int left, int right);
	int getIndexPrefixFromRowToBeOverriden(const Indices& indices, int left, int right);
	void precalculateFreshRow( const Indices& indices, Precalculations& precalculations );
	void calculateFreshSuffixes( const Indices& indices, Precalculation& precalculation, int row );
	void calculateFreshPrefixes( const Indices& indices, Precalculation& precalculation, int row );
};