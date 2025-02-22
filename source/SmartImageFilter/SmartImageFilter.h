#pragma once
#include "../ImageFilter/ImageFilter.h"
#include <vector>

class SmartImageFilter : public ImageFilter {

struct Coordinates{
	int row;
	int column;
};

struct Indices
{
	std::vector< Coordinates > extremaCoordinates;
	struct MaskIndices
	{
		int left;
		int right;
		int center;
	} firstMask, secondMask;
	int top;
	int bot;
	int row;
	
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

	int getIndexPrefix( const Indices& indices, int left, int right, int row );
	int getIndexSuffix( const Indices& indices, int left, int right, int row );

	void calculatePrefixes( const Indices& indices, Precalculation& precalculation, int row );

	void calculateSuffixes( const Indices& indices, Precalculation& precalculation, int row );

	void precalculate( const Indices& indices, Precalculations& precalculations );
	void precalculateOnlyLast( const Indices& indices, Precalculations& precalculations );

	void setPrefixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculation& precalculation );

	void setAffixMixMaskExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void setSuffixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculation& precalculation );

	void setExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void updateRowColumnAndIndex( int& row, int& column, int& index );

	void updateIndices( Indices& indices, int row, int column );
};
