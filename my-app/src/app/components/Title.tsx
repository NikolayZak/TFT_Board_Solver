import Image from "next/image";

export const Title = () => {
    return(
        <div className="flex justify-center items-center">
            <Image src="/logo.png" alt="" width={50} height={50} className="mr-3" />
            <h1 className="text-heading">TFT Solver</h1>
        </div>
    );
}