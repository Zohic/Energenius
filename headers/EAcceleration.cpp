#include "EAcceleration.h"
using namespace concurrency;

EAcceleration::EAcceleration()
{
	accelerators = Concurrency::accelerator::get_all();
}

bool EAcceleration::ComputeTextureMat3(IVec2d begin, IVec2d size, IVec2d osize, EColor* pix, EColor* orig, Mat33& mat)
{
	//extent<2> matExt(3, 3);
	extent<2> vecExt(size.x * size.y, 2);
	extent<1> pixExt(size.x * size.y);
	extent<1> origExt(osize.x * osize.y);

	std::vector<int> out;
	out.resize(size.x * size.y * 2);


	/*std::vector<float> matVec{ 
		mat.mat[0][0], mat.mat[0][1],mat.mat[0][2],
		mat.mat[1][0], mat.mat[1][1],mat.mat[1][2],
		mat.mat[2][0], mat.mat[2][1],mat.mat[2][2]};*/
	//array_view<float, 2> matView(matExt, matVec);

	array_view<int, 2> vecViewOut(vecExt, out);
	array_view<EColor, 1> pixView(pixExt, pix);
	array_view<EColor, 1> origView(origExt, orig);

	vecViewOut.discard_data();

	parallel_for_each(vecViewOut.extent, [=](index<2> ind)restrict(amp) {
		
		int i = ind[0];
		int j = ind[1];
		if (j % 2 == 0)
			vecViewOut[ind] = (begin.x + i % size.x) * mat.mat[0][0] + (begin.y + i / size.x) * mat.mat[0][1] + mat.mat[0][2];
		else
		{
			vecViewOut[ind] = (begin.x + i % size.x) * mat.mat[1][0] + (begin.y + i / size.x) * mat.mat[1][1] + mat.mat[1][2];
			pixView(i).data = origView(vecViewOut(i, 0) + vecViewOut(i, 1) * osize.x).data;
		}
		});
	pixView.synchronize();
	return true;

}

