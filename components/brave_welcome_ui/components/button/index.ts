/* This Source Code Form is subject to the terms of the Mozilla Public
 * License. v. 2.0. If a copy of the MPL was not distributed with this file.
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { ComponentType } from 'react'
import styled, { css } from 'brave-ui/theme'
import Button, { Props as ButtonProps } from 'brave-ui/components/buttonsIndicators/button'

interface BaseButtonProps {
  active?: boolean
}

const BaseButton = styled<BaseButtonProps, 'button'>('button')`
  box-sizing: border-box;
  padding: 0;
  margin: 0;
  display: block;
  line-height: 1;
  background: none;
  border: none;
  cursor: pointer;
  outline: none;
`

export const FooterButton = styled(Button as ComponentType<ButtonProps>)`
  outline: none;
  border: 1px solid ${p => p.theme.palette.grey400};
  color: ${p => p.theme.color.text};

  &:hover {
    opacity: .9;
  }

  &:focus {
    box-shadow: 0 0 0 2px rgba(255,80,0,0.2);
  }
`

export const SkipButton = styled(BaseButton)`
  color: ${p => p.theme.color.text};
  text-decoration: underline;
  font-weight: 300;
  letter-spacing: 0;

  &:hover {
    opacity: .9;
  }

  &:focus {
    box-shadow: 0 0 0 2px rgba(33,37,41,0.2);
  }
`

export const PrimaryButton = styled(Button as ComponentType<ButtonProps>)`
  outline: none;

  &:hover {
    opacity: .9;
  }

  &:focus {
    box-shadow: 0 0 0 2px rgba(255,80,0,0.2);
  }
`

export const Bullet = styled(BaseButton as ComponentType<any>)`
  margin: 0 7px;
  width: 11px;
  height: 11px;
  color: #7C7D8C;
  letter-spacing: 0;

  &:hover {
    color: #343546;
  }

  &:focus {
    box-shadow: 0 0 0 2px rgba(255,80,0,0.2);
    border-radius: 50%;
  }

  ${p => p.active && css`
    color: #FB542B;

    &:hover {
      color: #C72E03;
    }
  `}
`
