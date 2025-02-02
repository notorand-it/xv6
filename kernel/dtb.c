#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "dtb.h"

//https://stackoverflow.com/questions/68234769/how-does-qemu-riscv-pass-the-device-tree-blob-to-the-guest-kernel


void parse_fdt_reserve_entries(struct fdt_header* header)
{
    uint64 offset = fdt32_to_cpu(header->off_mem_rsvmap);
    struct fdt_reserve_entry* entry = (struct fdt_reserve_entry*)((uint64)header+offset);
    while (1)
    {
        uint64 address = fdt64_to_cpu(entry->address);
        uint64 size = fdt64_to_cpu(entry->size);
        if(address == 0 && size == 0)
        {
            break;
        }
        printf("Reserved memory : Address = 0x%lx, Size = 0x%lx\n",address,size);
        entry++;
    }
    
}

void print_deep(int deep)
{
    while (deep--)
    {
        printf("  |");
    }
    printf("-");
    
}
void parse_fdt_property(struct fdt_property* property,char* strings,int deep)
{
    uint32 length = fdt32_to_cpu(property->len);
    char* name = strings+fdt32_to_cpu(property->nameoff);
    char* value = property->data;
    print_deep(deep);
    printf("%s\n",name);
    if(length==0)
    {
        print_deep(deep);
        printf("=value : (null)\n");
    }else if(length % (2 * sizeof(uint64)) == 0)
    {
        uint32 num_pairs = length / (2 * sizeof(uint64));
        for (int i = 0; i < num_pairs; i++)
        {
            uint64 base = fdt64_to_cpu(*(uint64*)(value + i * 2 * sizeof(uint64)));
            uint64 size = fdt64_to_cpu(*(uint64*)(value + (i * 2 + 1) * sizeof(uint64)));
            print_deep(deep);
            printf("=value: <0x%lx 0x%lx>\n", base, size);
        } 
    }else if(length % (2 * sizeof(uint32)) == 0)
    {
        uint32 num_pairs = length / (2 * sizeof(uint32));
        for (int i = 0; i < num_pairs; i++)
        {
            uint32 base = fdt32_to_cpu(*(uint32*)(value + i * 2 * sizeof(uint32)));
            uint32 size = fdt32_to_cpu(*(uint32*)(value + (i * 2 + 1) * sizeof(uint32)));
            print_deep(deep);
            printf("=value: <0x%x 0x%x>\n", base, size);
        } 
    }else if(length >0 && value[length - 1]=='\0')
    {
        print_deep(deep);
        printf("=value : \"%s\"\n",value);
    }else{
        print_deep(deep);
        printf("=value: unknown\n");
    }
}

void parse_device_tree_blob(struct fdt_header* header)
{
    if(fdt32_to_cpu(header->magic) != FDT_MAGIC)
    {
        panic("parse_device_tree_blob");
    }


    uint64 offset = fdt32_to_cpu(header->off_dt_struct);
    char* strings = (char*)(((uint64)header)+fdt32_to_cpu(header->off_dt_strings));
    int deep = 0;
    uint32 tag=0;
    
    do{
        tag = fdt32_to_cpu(*(uint32*)((uint64)header+offset));
        if(tag == FDT_BEGIN_NODE)
        {
            char* node_name = (char*)((uint64)header+offset+sizeof(uint32));
            print_deep(deep);
            printf("node \"%s\" \n",node_name);
            deep++;
            offset+=sizeof(uint32); // skip tag
            offset+= strlen((char*)((uint64)header+offset))+1; // skip name;
            offset= (offset + 3) & ~3;// align position
        }else if(tag == FDT_END_NODE)
        {
            //print_deep(deep);
            //printf("node end\n");
            deep--;
            offset+=sizeof(uint32);
        }else if(tag == FDT_PROP)
        {
            struct fdt_property* property = (struct fdt_property*)((uint64)header+offset);
            parse_fdt_property(property,strings,deep);
            offset+=sizeof(struct fdt_property); // skip fdt_property
            offset+=fdt32_to_cpu(property->len); // skip value
            offset = (offset + 3) & ~3; // align
        }else if(tag == FDT_NOP)
        {
            offset+=sizeof(uint32);
        }
        else if(tag == FDT_END)
        {
            break;
        }
        else{
            //printf("unknown\n");
            offset+=sizeof(uint32);
        }
    }while(tag != FDT_END);
    
}

uint32 get_fdt_node_offset(struct fdt_header* header,char* name,uint32 sz)
{
    uint64 offset = fdt32_to_cpu(header->off_dt_struct);
    //char* strings = (char*)(((uint64)header)+fdt32_to_cpu(header->off_dt_strings));
    uint32 tag=0;
    while (1)
    {
        tag = fdt32_to_cpu(*(uint32*)((uint64)header+offset));
        if(tag == FDT_BEGIN_NODE)
        {
            char* node_name = (char*)((uint64)header+offset+sizeof(uint32));
            if(strncmp(name,node_name,sz)==0)
            {
                return offset;
            }
            offset+=sizeof(uint32); // skip tag
            offset+= strlen((char*)((uint64)header+offset))+1; // skip name;
            offset= (offset + 3) & ~3;// align position
        }else if(tag == FDT_END_NODE)
        {
            offset+=sizeof(uint32);
        }else if(tag == FDT_PROP)
        {
            struct fdt_property* property = (struct fdt_property*)((uint64)header+offset);
            offset+=sizeof(struct fdt_property); // skip fdt_property
            offset+=fdt32_to_cpu(property->len); // skip value
            offset = (offset + 3) & ~3; // align
        }else if(tag == FDT_NOP)
        {
            offset+=sizeof(uint32);
        }
        else if(tag == FDT_END)
        {
            break;
        }
        else{
            //printf("unknown\n");
            offset+=sizeof(uint32);
        }
    }
    return 0;
    
}

uint32 get_fdt_property_offset(struct fdt_header* header, uint32 node_offset,char* name, uint32 sz)
{
    uint32 offset = node_offset;
    char* strings = (char*)(((uint64)header)+fdt32_to_cpu(header->off_dt_strings));
    uint32 tag = 0;
    int deep = 1;
    while (1)
    {
        tag = fdt32_to_cpu(*(uint32*)((uint64)header+offset));
        if(tag == FDT_BEGIN_NODE)
        {
            offset+=sizeof(uint32); // skip tag
            offset+= strlen((char*)((uint64)header+offset))+1; // skip name;
            offset= (offset + 3) & ~3;// align position
            deep++;
        }else if(tag == FDT_END_NODE)
        {
            offset+=sizeof(uint32);
            deep--;
            if(deep == 0)
            {
                break;
            }
        }else if(tag == FDT_PROP)
        {
            struct fdt_property* property = (struct fdt_property*)((uint64)header+offset);
            char* prop_name = strings + fdt32_to_cpu(property->nameoff);
            if(strncmp(prop_name,name,sz)==0)
            {
                return offset;
            }
            offset+=sizeof(struct fdt_property); // skip fdt_property
            offset+=fdt32_to_cpu(property->len); // skip value
            offset = (offset + 3) & ~3; // align
        }else if(tag == FDT_NOP)
        {
            offset+=sizeof(uint32);
        }
        else if(tag == FDT_END)
        {
            break;
        }
        else{
            //printf("unknown\n");
            offset+=sizeof(uint32);
        }
    }
    return 0;
    
}

void* get_fdt_propert_value(struct fdt_header* header,uint32 node_offset,char* name, uint32 sz,uint32* length)
{
    uint32 prop_offset = get_fdt_property_offset(header,node_offset,name,sz);
    struct fdt_property* property = (struct fdt_property*)((uint64)header+prop_offset);
    *length = property->len;
    return property->data;
}


void dtbinit()
{
    struct fdt_header* fdt = (struct fdt_header*)fdt_address;
    uint32 node = get_fdt_node_offset(fdt,"memory",6);
    if(node==0) panic("get_fdt_node_offset");
    uint32 len=0;
    void* value = get_fdt_propert_value(fdt,node,"reg",3,&len);
    if(value==0) panic("get_fdt_propert_value");
    uint64 base = fdt64_to_cpu(*(uint64*)value);
    uint64 size = fdt64_to_cpu(*(uint64*)(value+sizeof(uint64)));
    printf("memory : 0x%lx+0x%lx\n",base,size);
}

