import path from 'node:path';

import type {
    Address,
    ProcessArchitecture
} from './types';

const addon = require(
    path.join(
        __dirname,
        '..',
        'build',
        'Release',
        'memkitty.node'
    )
);

export class ProcessManager {
    private readonly nativeProcess: any;

    constructor(
        pid: number,
        architecture: ProcessArchitecture = 'auto'
    ) {
        this.nativeProcess =
            new addon.NativeProcess(
                pid,
                architecture
            );
    }

    static getPidsByName(
        processName: string
    ): number[] {
        return addon.getPidsByName(
            processName
        );
    }

    static getPidsByTitle(
        windowTitle: string
    ): number[] {
        return addon.getPidsByTitle(
            windowTitle
        );
    }

    static getHwndByPid(
        pid: number
    ): bigint {
        return addon.getHwndByPid(
            pid
        );
    }

    static postMessage(
        hwnd: bigint,
        message: number,
        wParam?: number,
        lParam?: number,
    ): boolean {
        return addon.postMessage(hwnd, message, wParam, lParam);
    }

    static makeLParam(x: number, y: number): number {
        return ((y & 0xffff) << 16) | (x & 0xffff);
    }

    open(): boolean {
        return this.nativeProcess.open();
    }

    close(): void {
        this.nativeProcess.close();
    }

    isOpen(): boolean {
        return this.nativeProcess.isOpen();
    }

    getArchitecture(): ProcessArchitecture {
        return this.nativeProcess.getArchitecture();
    }

    getBaseAddress(): Address {
        return this.nativeProcess.getBaseAddress();
    }

    readInt32(address: Address): number {
        return this.nativeProcess.readInt32(
            address
        );
    }

    readUInt32(address: Address): number {
        return this.nativeProcess.readUInt32(
            address
        );
    }

    readInt64(address: Address): bigint {
        return this.nativeProcess.readInt64(
            address
        );
    }

    readUInt64(address: Address): bigint {
        return this.nativeProcess.readUInt64(
            address
        );
    }

    readFloat(address: Address): number {
        return this.nativeProcess.readFloat(
            address
        );
    }

    readPointer(address: Address): Address {
        return this.nativeProcess.readPointer(
            address
        );
    }

    readBytes(
        address: Address,
        size: number
    ): Buffer {
        return this.nativeProcess.readBytes(
            address,
            size
        );
    }

    readWString(
        address: Address,
        length: number
    ): string {
        return this.nativeProcess.readWString(
            address,
            length
        );
    }

    allocateMemory(size: number): Address {
        return this.nativeProcess.allocateMemory(
            size
        );
    }

    inject(
        address: Address,
        data: Buffer
    ): boolean {
        return this.nativeProcess.inject(
            address,
            data
        );
    }

    injectAndExecute(
        address: Address,
        data: Buffer
    ): boolean {
        return this.nativeProcess.injectAndExecute(
            address,
            data
        );
    }
}