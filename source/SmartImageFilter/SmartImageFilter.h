#pragma once
#include "../ImageFilter/ImageFilter.h"

class SmartImageFilter : public ImageFilter {

struct Indices
{
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
using Precalculations = std::vector< int >;

public:
	SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize);
	void apply(cv::Mat& image) override;
private:
	void filter(cv::Mat& newImage, int firstIndex, int lastIndex);

	void calculatePrefixes( const Indices& indices, Precalculations& precalculations, int row );

	void calculateSuffixes( const Indices& indices, Precalculations& precalculations, int row );

	void precalculate( const Indices& indices, Precalculations& precalculations );

	void setPrefixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void setAffixMixMaskExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void setSuffixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void setExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations );

	void updateRowColumnAndIndex( int& row, int& column, int& index );

	void updateIndices( Indices& indices, int row, int column );
};
