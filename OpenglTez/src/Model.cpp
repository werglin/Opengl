#include"Model.hpp"

Model::Model()
{

}
void Model::Init()
{

}
void Model::Render(ShaderProgram* shader)
{
	for(auto &var : _meshes)
	{
		var.Render(shader);
	}
}
void Model::CleanUp()
{
	for (auto& var : _meshes)
	{
		var.CleanUp();
	}
}