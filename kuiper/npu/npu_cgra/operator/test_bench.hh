#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

namespace gem5 {

#define REQUEST_WIDTH 1024
#define REQ_WIDTH_BIT 128
#define MEM_SIZE		(1024 * 1024 * 128)
#define LOADADDR0	0
#define LOADADDR1	1024 * 1024 * 32
#define STOREADDR	1024 * 1024 * 64
#define BIT16 0
#define BIT32 1

	template <typename KuiperCgraType>
	class Cgra
	{
	public:
		explicit Cgra( KuiperCgraType *obj) : mObj(obj) { }
		~Cgra() {}
		enum Lmop {
			UnitStride,
			Stride,
			Index,
			TwoD
		};

		struct loadInstruction {
			uint32_t imop : 2;
			uint32_t ilumop : 1;
			bool ivm : 1;
			bool imew : 1;
			uint32_t ivlen;
			uint32_t istride;
			uint32_t ieew;
		};

		struct Data1024bit {
			unsigned char data[128];
		};

		class Load {
		public:
			bool reset;
			uint32_t s_address;
			uint32_t t_address;
			uint32_t vlen;
			uint32_t stride;
			bool vm;
			bool mew;
			uint32_t mop;
			uint32_t lumop;
			uint32_t eew;
			uint32_t minst;
			uint32_t datalen_byte;

			Data1024bit buffer[16];
			Data1024bit align_buffer;

		public:
			void unitStrideLoad(unsigned char* buf, std::vector<unsigned char>& vector, uint32_t index) {
				if (index == minst - 1) {
					uint32_t temp = (vlen - REQUEST_WIDTH / eew * index);
					uint32_t i = 0;
					for (i = 0; i < temp * eew / 8; i++) {
						align_buffer.data[i] = buf[i];
					}
					std::memcpy(vector.data() + index * REQ_WIDTH_BIT, align_buffer.data, temp * eew / 8);
				}
				else {
					for (uint32_t i = 0; i < REQ_WIDTH_BIT; i++) {
						align_buffer.data[i] = buf[i];
					}
					std::memcpy(vector.data() + index * REQ_WIDTH_BIT, align_buffer.data, REQ_WIDTH_BIT);
				}
			}
			void strideLoad(unsigned char* buf, std::vector<unsigned char>& vector, uint32_t index, uint32_t elem_offset) {
				uint32_t max_elem_num = REQUEST_WIDTH / Load::eew;
				static uint32_t num = 0;

				for (int i = 0; (elem_offset + i * Load::stride) < max_elem_num; i++) {
					std::memcpy(&vector.data()[num * Load::eew / 8], &buf[elem_offset + i * Load::stride], Load::eew / 8);
					num++;
				}
				if (num == Load::vlen - 1)
					num = 0;
			}
			void analyMinst(loadInstruction inst) {
				Load::vm = inst.ivm;
				Load::mew = inst.imew;
				Load::lumop = inst.ilumop;
				Load::mop = inst.imop;
				Load::stride = inst.istride;
				Load::vlen = inst.ivlen;
				Load::eew = inst.ieew;
				/*
				if (mop == UnitStride) {
					addr = addr - (addr % REQ_WIDTH_BIT);
					elem_offset = addr % REQ_WIDTH_BIT;
				}
				*/
				if (Load::vlen * Load::eew % REQUEST_WIDTH)
					Load::minst = Load::vlen * Load::eew / REQUEST_WIDTH + 1;
				else
					Load::minst = Load::vlen * Load::eew / REQUEST_WIDTH;
			}

		};

		enum class AdderType {
			FP32,
			FP16,
			BF16
		};

		enum class AdderFormat {
			V_VV,
			V_VS,
			V_VV_REDUCE
		};

		class VectorAdder {
		public:

			// V + V
			template <typename T>
			static std::vector<T> add(const std::vector<T>& v1, const std::vector<T>& v2, uint32_t len) {
				std::vector<T> result(len);

				for (uint32_t i = 0; i < len; ++i) {
					result[i] = v1[i] + v2[i];
					//           std::cout << result[i] << " = " << v1[i] << " + " << v2[i] << std::endl;
				}

				return result;
			}

			// V + S
			template <typename T>
			static std::vector<T> add(const std::vector<T>& v, T scalar, uint32_t len) {
				std::vector<T> result(len);
				for (uint32_t i = 0; i < len; i++) {
					result[i] = v[i] + scalar;
				}
				return result;
			}

			// REDUCE
			template<typename T>
			static T reduceAdd(const std::vector<T>& vec, uint32_t len) {
				T sum = T();

				for (uint32_t i = 0; i < len; i++) {
					sum += vec[i];
				}
				return sum;
			}

			void VAdd(std::vector<unsigned char>& vector0, std::vector<unsigned char>& vector1, uint32_t len, std::vector<unsigned char>& dvector)
			{
				if (type == AdderType::FP32) {
					uint32_t sew = 4;
					switch (format) {
					case AdderFormat::V_VV: {
						std::vector<float> result = VectorAdder::Add_VV<float>(vector0, vector1, len, dvector, sew);

						dvector.resize(len * sew);
						std::memcpy(dvector.data(), result.data(), len * sew);
						break;
					}

					case AdderFormat::V_VS: {
						std::cerr << "NO FORMAT!" << std::endl;
						break;
					}

					case AdderFormat::V_VV_REDUCE: {
						std::cerr << "NO FORMAT!" << std::endl;
						break;
					}
					default:
						std::cerr << "NO FORMAT!" << std::endl;
						break;
					}
				}
			}

			template <typename T>
			static std::vector<T> Add_VV(std::vector<unsigned char>& vec0, std::vector<unsigned char>& vec1, uint32_t len, std::vector<unsigned char>& dvec, uint32_t sew) {
				std::vector<T> v0(len);
				std::memcpy(v0.data(), vec0.data(), len * sew);
				std::vector<T> v1(len);
				std::memcpy(v1.data(), vec1.data(), len * sew);
				//std::vector<T> result(len) = VectorAdder::add<T>(v0, v1, len);
				return VectorAdder::add<T>(v0, v1, len);
			}

		public:
			AdderType type;
			AdderFormat format;
		};

		void GenMinst(loadInstruction& inst) {
			inst.imop = UnitStride;
			inst.ilumop = 0;
			inst.imew = BIT32;
			inst.ivlen = rand() % 300;
			if (inst.imew)
				inst.ieew = 32;
			else
				inst.ieew = 16;

		}
		void CheckStatus(unsigned char* buf0, unsigned char* buf1)
		{
			static uint32_t buf_index0 = 0;
			static uint32_t buf_index1 = 0;

			while (true) {
				memcpy(&buf_index0, &buf0[REQ_WIDTH_BIT], 4);
				memcpy(&buf_index1, &buf1[REQ_WIDTH_BIT], 4);
				if (buf_index0 == minst_index0 && buf_index1 == minst_index1) {
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		void OperationLoad(unsigned char* buf, Load& load, std::vector<unsigned char>& vector, uint32_t index, uint32_t elem_offset)
		{
			switch (load.mop) {
			case UnitStride: {
				load.unitStrideLoad(buf, vector, index);
				break;
			}
			case Stride: {
				load.strideLoad(buf, vector, index, elem_offset);
				break;
			}
			default:
				break;
			}
		}

		void Add(void)
		{
			uint32_t freq = 1;
			uint64_t taddr = STOREADDR;

			while (freq--) {

				uint32_t addr0 = LOADADDR0;
				uint32_t addr1 = LOADADDR1;

				unsigned char* buf0 = new unsigned char[132];
				unsigned char* buf1 = new unsigned char[132];
				uint32_t elem_offset0 = 0;
				uint32_t elem_offset1 = 0;

				loadInstruction load_inst;
				GenMinst(load_inst);

				Load load0;
				Load load1;
				load0.analyMinst(load_inst);
				load1.analyMinst(load_inst);

				std::vector<unsigned char> vector0(load0.vlen * load0.eew / 8, 0);
				std::vector<unsigned char> vector1(load0.vlen * load0.eew / 8, 0);

				//std::cout << "Start of Load" << std::endl;

				for (uint32_t i = 0; i < load0.minst; i++) {
					std::uint64_t temp_addr0 = addr0 + i * REQ_WIDTH_BIT;
					//std::cout << "tem_addr0 = " << temp_addr0 << std::endl;
					mObj->Load0(temp_addr0, buf0);
					std::uint64_t temp_addr1 = addr1 + i * REQ_WIDTH_BIT;
					mObj->Load1(temp_addr1, buf1);

					CheckStatus(buf0, buf1);
					OperationLoad(buf0, load0, vector0, minst_index0, elem_offset0);
					OperationLoad(buf1, load1, vector1, minst_index1, elem_offset1);
					minst_index0++;
					minst_index1++;
				}

				//std::cout << "End of Load" << std::endl;

				VectorAdder vadd;
				vadd.type = AdderType::FP32;
				vadd.format = AdderFormat::V_VV;
				std::vector<unsigned char> dvector(load_inst.ivlen * load_inst.ieew / 8, 0);
				vadd.VAdd(vector0, vector1, load_inst.ivlen, dvector);

				//uint32_t taddr = std::rand() % (MEM_SIZE / REQ_WIDTH_BIT) * REQ_WIDTH_BIT + MEM_SIZE;
				//if (freq == 10) {
				if (freq == 0) {
					target_address = taddr;
				}
				for (uint32_t i = 0; i < load0.minst; i++) {
					mObj->Store(taddr, dvector.data() + i * REQ_WIDTH_BIT);
					taddr += 128;
				}

				taddr += load_inst.ivlen * load_inst.ieew / 8;
				delete[] buf0;
				delete[] buf1;
			}
			//		check();
		}

	private:
		KuiperCgraType* mObj;
		uint32_t minst_index0 = 0;
		uint32_t minst_index1 = 0;
		std::uint64_t target_address = 0;
	};
}

#endif