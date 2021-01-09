/*!*******************************************************************
\file		  Render.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
********************************************************************/
#include <stdexcept>

#include "Render.h"

#include "SquareMesh.h"
#include "TriangleMesh.h"

namespace
{
	GLuint VAO[3] = {};
	GLuint VBO[3] = {};
	GLuint EBO[2] = {};

	constexpr int log_length = 512;
}

Render::~Render()
{
	glDeleteProgram(m_program);
	glUseProgram(0);

	for (int i = 0; i < 2; ++i)
		glDeleteBuffers(i, VBO);
}

Render::Render(float width, float height)
	: m_WidthHeight{ width, height }
{
	CompileShader();
	Initialize();
}

void Render::Initialize()
{
	// Set Attributes
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(2, EBO);

	// Triangle
	TriangleMesh triangle;
	const auto triangleVertexCount = triangle.GetVertexCount();

	glBindVertexArray(VAO[0]);

	// Allocate memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * triangleVertexCount, triangle.GetVertices(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	// size = 2 : vec2 (GL_FLOAT * 2)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * triangleVertexCount, triangle.GetFaces(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Square
	SquareMesh square;
	const auto squareVertexCount = square.GetVertexCount();

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * squareVertexCount, square.GetVertices(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * squareVertexCount, square.GetFaces(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Line
	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * 2, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::CompileShader()
{
	int success;
	char info_log[log_length];

	// Compile vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertex_shader_source = R"(
		#version 400
		layout(location = 0) in vec2 position;

		out vec3 frag_color;

		uniform vec2 WidthHeight;
		uniform vec3 color;

		float x = position.x / WidthHeight.x * 2.0;
		float y = position.y / WidthHeight.y * 2.0;

		void main() {
			gl_Position = vec4(x, y, 0.0, 1.0);
			frag_color = color;
		}
	)";

	glShaderSource(vertex_shader, 1, &vertex_shader_source, 0);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, log_length, 0, info_log);
		glDeleteShader(vertex_shader);
		throw(std::runtime_error(info_log));
	}

	// Compile fragment shader
	unsigned int fragement_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragment_shader_source = R"(
		#version 400
		in vec3 frag_color;

		void main() {
			gl_FragColor = vec4(frag_color, 1.0);
		}
	)";

	glShaderSource(fragement_shader, 1, &fragment_shader_source, 0);
	glCompileShader(fragement_shader);

	glGetShaderiv(fragement_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragement_shader, log_length, 0, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragement_shader);
		throw(std::runtime_error(info_log));
	}

	// Link Shaders
	m_program = glCreateProgram();

	glAttachShader(m_program, vertex_shader);
	glAttachShader(m_program, fragement_shader);
	glLinkProgram(m_program);

	glGetShaderiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_program, log_length, 0, info_log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragement_shader);
		glDeleteProgram(m_program);
		throw(std::runtime_error(info_log));
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragement_shader);

	glUseProgram(m_program);

	uWidthHeight = glGetUniformLocation(m_program, "WidthHeight");
	uColor = glGetUniformLocation(m_program, "color");
}

void Render::ClearBuffers()
{
	glClearColor(WHITE.r, WHITE.g, WHITE.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::DrawMesh(const Mesh& mesh) const
{
	glUseProgram(m_program);

	glBindVertexArray(VAO[mesh.GetType()]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[mesh.GetType()]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * mesh.GetVertexCount(), mesh.GetVertices());

	glUniform2fv(uWidthHeight, 1, m_WidthHeight);
	
	Color color = mesh.GetColor();
	float c[3] = { color.r, color.g, color.b };
	glUniform3fv(uColor, 1, c);

	glDrawElements(GL_TRIANGLES, mesh.GetFaceCount() * 3, GL_UNSIGNED_INT, 0);
}

void Render::DrawLine(const Point& start, const Point& end, const Color& color, float width)
{
	glUseProgram(m_program);

	glBindVertexArray(VAO[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

	Point points[2] = { start, end };
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * 2, points);

	glUniform2fv(uWidthHeight, 1, m_WidthHeight);

	float c[3] = { color.r, color.g, color.b };
	glUniform3fv(uColor, 1, c);

	glLineWidth(width);
	glDrawArrays(GL_LINES, 0, 2);
}