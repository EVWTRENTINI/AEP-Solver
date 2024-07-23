#pragma once
#include <unordered_map>
#include <cstring>
#include <cstdio>

// My includes
#include "interface/globals.h"
#include "interface/utils.h"
#include "interface/beam.h"
#include "interface/node.h"

// Eigen
#include "eigenpch.h"



class StructuralAnalysis{
public:
enum class Info
	{
        Success,
        Fail,
	};
size_t gln = 3; // Número de graus de liberdade por nó
std::vector<std::shared_ptr<Node>>* nodes;
std::vector<Beam>* beams;
Eigen::MatrixXi bcn; // Beam code number - Cada linha é uma barra, cada coluna é a tradução do grau de liberdade local para global
std::unordered_map<std::shared_ptr<Node>, int> nodeId; // Mapeamento de nós para índices
std::vector<Eigen::SparseMatrix<double>> k; // Matriz de rigidez de cada barra em coordenadas locais
std::vector<Eigen::SparseMatrix<double>> T; // Matriz de transformação
std::vector<Eigen::SparseMatrix<double>> K; // Matriz de rigidez de cada barra em coordenadas globais
Eigen::SparseMatrix<double> S; // Matriz de rigidez da estrutura
Eigen::VectorXd jl; // Vetor de carregamento em nós da estrutura em coordenadas globais
Eigen::VectorXd P; // Vetor de carregamento total da estrutura em coordenadas globais
Eigen::VectorXd d; // Vetor de deslocamentos da estrutura em coordenadas globais
Info info;
Eigen::MatrixXd v; // Deslocamento de cada barra em coordenadas globais
Eigen::MatrixXd F; // Forças nas extemidades das barras em coordenadas globais
Eigen::VectorXd R; // Vetor de reações de apoio
std::vector<std::vector<float>> Normal;
std::vector<std::vector<float>> Shear;
std::vector<std::vector<float>> Bending;
Eigen::MatrixXd Q; // Forças nas extemidades das barras em coordenadas locais


StructuralAnalysis() = default;


    
void Run(std::vector<std::shared_ptr<Node>>* newNodes, std::vector<Beam>* newBeams) {
    nodes = newNodes;
    beams = newBeams;

    nodeId.clear();
    k.clear();
    T.clear();
    K.clear();

    bcn.resize(beams->size(), 3*2);
    k.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    T.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    K.resize(beams->size(), Eigen::SparseMatrix<double>(3*2, 3*2));
    S=(Eigen::SparseMatrix<double>(3 * nodes->size(), 3 * nodes->size()));
    jl=(Eigen::VectorXd(3 * nodes->size()));
    P=(Eigen::VectorXd(3 * nodes->size()));
    d=(Eigen::VectorXd(3 * nodes->size()));
    v=(Eigen::MatrixXd(3*2, beams->size()));
    F=(Eigen::MatrixXd(3*2, beams->size()));
    R=(Eigen::VectorXd(3 * nodes->size()));
    // Pensar se da para trocar todos esses 3 por gln
    
    if (nodes->empty() || beams->empty()) {
        info = Info::Fail;
        return;
    }
    
    
    for (size_t n = 0; n < nodes->size(); ++n) {
        nodeId[(*nodes)[n]] = n;
    }
    
    
    //bcn.resize(beams.size(), gln*2);
    for (size_t n = 0; n < beams->size(); ++n) {
        // Codigo de número das barras
        bcn(n, 0) = (nodeId[(*beams)[n].startNode] + 1)*gln - 3;
        bcn(n, 1) = (nodeId[(*beams)[n].startNode] + 1)*gln - 2;
        bcn(n, 2) = (nodeId[(*beams)[n].startNode] + 1)*gln - 1;
        bcn(n, 3) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 3;
        bcn(n, 4) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 2;
        bcn(n, 5) = (nodeId[(*beams)[n].endNode]   + 1)*gln - 1;

        // Matrix de rigidez da barra
        float E = (*beams)[n].material->E;
        float A = (*beams)[n].section->area;
        float I = (*beams)[n].section->inertia;
        float L = (*beams)[n].length;
        double Lexp2 = L*L;
        double Lexp3 = Lexp2*L;
        double sc1 =        E * A / L;
        double sc2 = 12.0 * E * I / Lexp3;
        double sc3 =  6.0 * E * I / Lexp2;
        double sc4 =  4.0 * E * I / L;
        double sc5 =  2.0 * E * I / L;

        // Matriz de Rigidez de cada barra
        k[n].insert(0, 0) =  sc1;                                                     k[n].insert(0, 3) = -sc1;




                                  k[n].insert(1, 1) =  sc2; k[n].insert(1, 2) =  sc3;                           k[n].insert(1, 4) = -sc2; k[n].insert(1, 5) =  sc3;
                                  


        
                                  k[n].insert(2, 1) =  sc3; k[n].insert(2, 2) =  sc4;                           k[n].insert(2, 4) = -sc3; k[n].insert(2, 5) =  sc5;
                                  


        
        k[n].insert(3, 0) = -sc1;                                                     k[n].insert(3, 3) =  sc1;
        


        
                                  k[n].insert(4, 1) = -sc2; k[n].insert(4, 2) = -sc3;                           k[n].insert(4, 4) =  sc2; k[n].insert(4, 5) = -sc3;
                                  


        
                                  k[n].insert(5, 1) =  sc3; k[n].insert(5, 2) =  sc5;                           k[n].insert(5, 4) = -sc3; k[n].insert(5, 5) =  sc4;


        // Matriz de transformação de cada barra - TEM ELEMENTOS TROCADOS POR CONTA DE QUE NA VISUALIZAÇÃO O y é POSITIVO PARA CIMA
        T[n].insert(0, 0) =  (*beams)[n].r(0, 0);
        T[n].insert(0, 1) = -(*beams)[n].r(0, 1);
        T[n].insert(1, 0) = -(*beams)[n].r(1, 0);
        T[n].insert(1, 1) =  (*beams)[n].r(1, 1);
        T[n].insert(2, 2) =  1.0;
        T[n].insert(3, 3) =  (*beams)[n].r(0, 0);
        T[n].insert(3, 4) = -(*beams)[n].r(0, 1);
        T[n].insert(4, 3) = -(*beams)[n].r(1, 0);
        T[n].insert(4, 4) =  (*beams)[n].r(1, 1);
        T[n].insert(5, 5) =  1.0;

        // Constroi a matriz de rigidez em coordenadas GLOBAIS
        K[n] = T[n].transpose() * k[n] * T[n]; // K = T' * k * T

        for (size_t i = 0; i < gln*2; ++i) {
            for (size_t j = 0; j < gln*2; ++j) {
                double valueToAdd = K[n].coeff(i, j);
                if (valueToAdd != 0.0) {
                    S.coeffRef(bcn(n, i), bcn(n, j)) += valueToAdd;
                }
            }
        }
    }

    jl.setZero();
    for (size_t n = 0; n < nodes->size(); ++n) {
        jl.coeffRef(n*gln)   = (*nodes)[n]->load.force.x;   //Força em x aplicado em nó
        jl.coeffRef(n*gln+1) = (*nodes)[n]->load.force.y; // Força em y aplicado em nó
        jl.coeffRef(n*gln+2) = (*nodes)[n]->load.moment;  // Momento aplicado em nó
    }

    P = jl; // Falta somar os carramentos distribuidos

    for (size_t n = 0; n < nodes->size(); ++n) {
         switch ((*nodes)[n]->support) {
		    case Support::None:
                break;
            
            case Support::Lx:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                break;

            case Support::Ly:
                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                break;

            case Support::Lxy:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                break;

            case Support::Th:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                zeroRow(S, n*gln + 2);
                zeroColumn(S, n*gln + 2);
                S.coeffRef(n*gln + 2, n*gln + 2) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                P.coeffRef(n*gln+2) = 0; // Momento aplicado em nó
                break;

            case Support::Tv:
                zeroRow(S, n*gln);
                zeroColumn(S, n*gln);
                S.coeffRef(n*gln, n*gln) = 1;

                zeroRow(S, n*gln + 1);
                zeroColumn(S, n*gln + 1);
                S.coeffRef(n*gln + 1, n*gln + 1) = 1;

                zeroRow(S, n*gln + 2);
                zeroColumn(S, n*gln + 2);
                S.coeffRef(n*gln + 2, n*gln + 2) = 1;

                P.coeffRef(n*gln) = 0;   //Força em x aplicado em nó
                P.coeffRef(n*gln+1) = 0; // Força em y aplicado em nó
                P.coeffRef(n*gln+2) = 0; // Momento aplicado em nó
                break;

			default:
		        std::cout << "Apoio desconhecido" << std::endl;
		        break;
		}



    }

    
    /*// IMPOSSÍVEL USAR ISSO, O TEMPO DE COMPILAÇÃO FICA 3 MINUTOS MAIOR!
    // Cria um solucionador de autovalores para matrizes não-simétricas
    Eigen::MatrixXd denseS = Eigen::MatrixXd(S);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(denseS);

    // Autovalores
    std::cout << "Autovalores:\n" << eigensolver.eigenvalues() << std::endl;
    // Autovetores
    std::cout << "Autovetores:\n" << eigensolver.eigenvectors() << std::endl;*/
    

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.analyzePattern(S);
    solver.factorize(S);
    if (solver.info() != Eigen::Success) {
        if (solver.info() == Eigen::NumericalIssue) {
            std::cerr << "Problema numérico detectado." << std::endl;
        } else if (solver.info() == Eigen::NoConvergence) {
            std::cerr << "O solver não convergiu." << std::endl;
        } else if (solver.info() == Eigen::InvalidInput) {
            std::cerr << "Entrada inválida para o solver." << std::endl;
        }
        return;
    }
    info = Info::Success;

    d = solver.solve(P);
    std::cout << "Deslocamentos (d):\n" << d << std::endl;
    

    R.setZero();
    for (size_t n = 0; n < beams->size(); ++n) {
        for (size_t i = 0; i < gln*2; ++i) {
            v(i, n) = d(bcn(n, i));
        }

        F.col(n) = K[n] * v.col(n);

        for (size_t i = 0; i < gln*2; ++i) {
            R(bcn(n,i)) += F(i, n);
        }
    }

    R = R - jl;



    std::cout << "Deslocamentos de cada barra (v):\n" << v << std::endl;
    std::cout << "Forças em cada barra (F):\n" << F << std::endl;
    std::cout << "Reações de apoio (R):\n" << R << std::endl;
}

void DrawReactions(Camera2D cam){
    for (size_t n = 0; n < nodes->size(); ++n) {
        float Rx = static_cast<float>(R(n * gln));
        drawFixedSizeAnnotadedArrow((*nodes)[n]->position, Vector2{Rx, 0}, loadScale, false, raioNo * 2 , raioNo / 1, raioNo * 0.5f, SKYBLUE, cam);

        float Ry = static_cast<float>(R(n * gln + 1));
        drawFixedSizeAnnotadedArrow((*nodes)[n]->position, Vector2{0, Ry}, loadScale, false, raioNo * 2 , raioNo / 1, raioNo * 0.5f, SKYBLUE, cam);

        float Mz = static_cast<float>(R(n * gln + 2));
        drawFixedSizeAnnotadedMoment((*nodes)[n]->position, Mz, raioNo * 5, raioNo * 2, raioNo / 1, raioNo * 0.5f, momentReactionColor, cam);
    }
}

void EvalInternalForces(){
    // Inicializa os vetores de vetores com o número de beams
    Normal.resize(beams->size());
    Shear.resize(beams->size());
    Bending.resize(beams->size());
    Q = (Eigen::MatrixXd(gln*2, beams->size()));

    // Preenche os vetores internos com os tamanhos apropriados para cada beam
    for (size_t n = 0; n < beams->size(); ++n) {
        // No momento o tamanho é 2 só, incial e final pois só temos forças aplicadas nos nós;
        Normal[n].resize(2);
        Shear[n].resize(2); 
        Bending[n].resize(2);
    }

    // Vetores com os valores calculados
    for (size_t n = 0; n < beams->size(); ++n) {
        //std::cout << "Barra " << n << std::endl;
        // Exemplo de preenchimento dos vetores com valores fictícios
        Q.col(n) = T[n] * F.col(n);

        Normal[n][0] = -Q(0,n); 
        Normal[n][1] = -Q(0,n);
        //std::cout << "N:\n" << Normal[n][0] << " " << Normal[n][1] << std::endl;

        
        Shear[n][0] = Q(1,n);
        Shear[n][1] = Q(1,n);
        //std::cout << "Q:\n" << Shear[n][0] << " " << Shear[n][1] << std::endl;
        
        Bending[n][0] = -Q(2,n);
        Bending[n][1] = -Q(2,n) + Q(1,n) * (*beams)[n].length;
        //std::cout << "M:\n" << Bending[n][0] << " " << Bending[n][1] << std::endl;
    }
}

void DrawDiagram(Camera2D cam){
float escale;
int signal = -1; // Trocado pq o raylib desenha negativo para cima
Color fillColor;
Color borderColor;
std::vector<std::vector<float>>* internalForce = nullptr;

switch (toolMode) {
       	case ToolMode::ShowNormal:
            escale = normalScale;
            fillColor = normalFill;
            borderColor = normalBorder;
            internalForce = &Normal;
		break;
        
		case ToolMode::ShowShear:
            escale = shearScale;
            fillColor = shearFill;
            borderColor = shearBorder;
            internalForce = &Shear;
			break;

		case ToolMode::ShowBending:
            escale = bendingScale;
            fillColor = bendingFill;
            borderColor = bendingBorder;
            internalForce = &Bending;
            signal = 1;
			break;
        
		default:
	        std::cout << "Modo desconhecido" << std::endl;
	        break;

	}
    if (internalForce != nullptr) {	
        for (size_t b = 0; b < beams->size(); ++b) {
            Vector2 firstBeamPoint;
            Vector2 lastBeamPoint;
            Vector2 p1, p2, p3, p4;
            for (size_t i = 0; i < (*internalForce)[b].size() - 1; ++i) {
                Eigen::Vector2d vp1(0, 0);
                Eigen::Vector2d vp2(0 , (*internalForce)[b][i] * signal * escale / cam.zoom);
                Eigen::Vector2d vp3((*beams)[b].length , (*internalForce)[b][i+1] * signal * escale  / cam.zoom);
                Eigen::Vector2d vp4((*beams)[b].length , 0);

                
                Eigen::Matrix2d r;
                r <<  (*beams)[b].r(0,0),  -(*beams)[b].r(0,1),
                     -(*beams)[b].r(1,0),  (*beams)[b].r(1,1);

                vp1 = r * vp1;
                vp2 = r * vp2;
                vp3 = r * vp3;
                vp4 = r * vp4;

                p1 = {static_cast<float>(vp1(0)), static_cast<float>(vp1(1))};
                p2 = {static_cast<float>(vp2(0)), static_cast<float>(vp2(1))};
                p3 = {static_cast<float>(vp3(0)), static_cast<float>(vp3(1))};
                p4 = {static_cast<float>(vp4(0)), static_cast<float>(vp4(1))};

                p1 = Vector2Add(p1, (*beams)[b].startNode->position);
                p2 = Vector2Add(p2, (*beams)[b].startNode->position);
                p3 = Vector2Add(p3, (*beams)[b].startNode->position);
                p4 = Vector2Add(p4, (*beams)[b].startNode->position);   

                BeginMode2D(cam);
                if ((*internalForce)[b][i] * (*internalForce)[b][i + 1] >= 0) {
                    // Sinal não mudou
                    if ((*internalForce)[b][i] * signal >= 0) {
                        // Desenha triângulos no sentido anti-horário
                        DrawTriangle(p1, p2, p3, fillColor);
                        DrawTriangle(p1, p3, p4, fillColor);
                    } else {
                        // Inverte a ordem dos vértices para garantir sentido anti-horário
                        DrawTriangle(p1, p3, p2, fillColor);
                        DrawTriangle(p1, p4, p3, fillColor);
                    }
                } else {
                    // Sinal mudou
                    float zeroCrossing = (*internalForce)[b][i] / ((*internalForce)[b][i] - (*internalForce)[b][i + 1]);
                    Eigen::Vector2d vpCross((*beams)[b].length * zeroCrossing, 0);

                    vpCross = r * vpCross;
                    Vector2 pCross = {static_cast<float>(vpCross(0)), static_cast<float>(vpCross(1))};
                    pCross = Vector2Add(pCross, (*beams)[b].startNode->position);

                    if ((*internalForce)[b][i] * signal >= 0) {
                        DrawTriangle(p1, p2, pCross, fillColor);
                    } else {
                        DrawTriangle(p1, pCross, p2, fillColor);
                    }

                    if ((*internalForce)[b][i + 1] * signal >= 0) {
                        DrawTriangle(pCross, p3, p4, fillColor);
                    } else {
                        DrawTriangle(pCross, p4, p3, fillColor);
                    }
                }

                DrawLineEx(p2, p3, raioNo * 0.5f / cam.zoom, borderColor);
                EndMode2D();

                if (i == 0){ // Se for o primerio ponto da barra
                    firstBeamPoint = p2;  
                }                
            }
            lastBeamPoint = p3;
            BeginMode2D(cam);
                DrawLineEx((*beams)[b].startNode->position, firstBeamPoint, raioNo * 0.5f / cam.zoom, borderColor);
                DrawLineEx(lastBeamPoint, (*beams)[b].endNode->position, raioNo * 0.5f / cam.zoom, borderColor);
            EndMode2D();
            


            // MUITO RUIMMMM - NEM TENTE ENTENDER OQ UE ESTA SENDO FEITO AQUI, REFAÇA!
            {
            char annotation[256];
            snprintf(annotation, sizeof(annotation), "%.2f", (*internalForce)[b].front());
            int intOffset = 5;
            Vector2 beamVector = Vector2Subtract((*beams)[b].endNode->position, (*beams)[b].startNode->position);
            Vector2 textSizeOri = MeasureTextEx(fontTtf, annotation, myFontSize, 1);
            Vector2 textSize = Vector2{textSizeOri.x, textSizeOri.y * .6f};
            Vector2 offsetParallelBeam = FindIntersection(textSize, beamVector);
            

            Vector2 size = Vector2Subtract(p2, (*beams)[b].startNode->position);
            Vector2 sizeFliped = size; // Inverte por que o raylib desenha positivo para baixo
            size = Vector2{sizeFliped.x, -sizeFliped.y}; // Inverte por que o raylib desenha positivo para baixo

            Vector2 offset = Vector2Scale(Vector2Normalize(sizeFliped), intOffset / cam.zoom);

            Vector2 textPositionWorld = Vector2{offset.x + p2.x,offset.y + p2.y};
            Vector2 textPosition = Vector2Subtract(GetWorldToScreen2D(textPositionWorld, cam), Vector2Scale(textSize, .5f));
            Vector2 offsetPerpendicularBeam = FindIntersection(textSize, size);
            textPosition = Vector2{textPosition.x + offsetPerpendicularBeam.x + offsetParallelBeam.x, textPosition.y - offsetPerpendicularBeam.y + offsetParallelBeam.y};
            //DrawRectangleLines(textPosition.x, textPosition.y, textSize.x, textSize.y, YELLOW);
	        DrawTextEx(fontTtf, annotation, Vector2{textPosition.x, textPosition.y - (textSizeOri.y - textSize.y)/2 - textSizeOri.y*.07f}, myFontSize, 1, borderColor);
            }
            {
            char annotation[256];
            snprintf(annotation, sizeof(annotation), "%.2f", (*internalForce)[b].back());
            int intOffset = 5;
            Vector2 beamVector = Vector2Subtract((*beams)[b].startNode->position, (*beams)[b].endNode->position);
            Vector2 textSizeOri = MeasureTextEx(fontTtf, annotation, myFontSize, 1);
            Vector2 textSize = Vector2{textSizeOri.x, textSizeOri.y * .6f};
            Vector2 offsetParallelBeam = FindIntersection(textSize, beamVector);
            

            Vector2 size = Vector2Subtract(p3, (*beams)[b].endNode->position);
            Vector2 sizeFliped = size; // Inverte por que o raylib desenha positivo para baixo
            size = Vector2{sizeFliped.x, -sizeFliped.y}; // Inverte por que o raylib desenha positivo para baixo

            Vector2 offset = Vector2Scale(Vector2Normalize(sizeFliped), intOffset / cam.zoom);

            Vector2 textPositionWorld = Vector2{offset.x + p3.x,offset.y + p3.y};
            Vector2 textPosition = Vector2Subtract(GetWorldToScreen2D(textPositionWorld, cam), Vector2Scale(textSize, .5f));
            Vector2 offsetPerpendicularBeam = FindIntersection(textSize, size);
            textPosition = Vector2{textPosition.x + offsetPerpendicularBeam.x + offsetParallelBeam.x, textPosition.y - offsetPerpendicularBeam.y + offsetParallelBeam.y};
            //DrawRectangleLines(textPosition.x, textPosition.y, textSize.x, textSize.y, YELLOW);
	        DrawTextEx(fontTtf, annotation, Vector2{textPosition.x, textPosition.y - (textSizeOri.y - textSize.y)/2 - textSizeOri.y*.07f}, myFontSize, 1, borderColor);
            }


        }
    }

}

};



